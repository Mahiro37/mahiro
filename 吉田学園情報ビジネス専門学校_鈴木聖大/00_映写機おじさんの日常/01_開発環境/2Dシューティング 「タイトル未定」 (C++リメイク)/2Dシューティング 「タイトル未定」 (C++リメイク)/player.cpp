//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "keyboard.h"
#include "controller.h"
#include "bullet.h"
#include "sound.h"
#include "enemy.h"
#include "explosion.h"
#include "item.h"
#include "effect.h"
#include "score.h"
#include "life.h"
#include "barrier.h"
#include "fade.h"
#include "time.h"
#include "score_attack.h"
#include "tutorial_score_attack.h"
#include "game.h"
#include "time.h"
#include "message.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define PLAYER_MOVE						(5.0f)							//�v���C���[�̈ړ���(�����l)
#define ANIMATION_SPEED					(6.0f)							//�A�j���[�V�����̑���
#define ANIMATION_PATTERN				(4.0f)							//�A�j���[�V�����̍s��
#define ANIMATION_LINE					(1.0f)							//�A�j���[�V�����̗�
#define ITEM_SCORE						(1000)							//�X�R�A�A�C�e���̃|�C���g��
#define PLAYER_SPEED_UP					(2.0f)							//���@�̃X�s�[�h�㏸��
#define BULLET_CHARGE_SPEED_DECREASE	(-(BULLET_CHARGE_SPEED / 5))	//�e�̃`���[�W�X�s�[�h������

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture[PLAYER_TEXTURE] = {};
int CPlayer::m_nLife = PLAYER_LIFE;
CBarrier *CPlayer::m_pBarrier = NULL;
bool CPlayer::m_bRelease = true;

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CPlayer::CPlayer()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PlayerState = PLAYER_STATE_NONE;
	m_nItemScore = 0;
	m_fPlayerSpeed = 0.0f;
	m_nChargeSpeed = 0;
	m_pBullet = NULL;
	m_pEffect = NULL;
	m_pMessage = NULL;
	m_nCntInvincible = 0;
	m_nInvincibleTime = 0;
	m_nCntPress = 0;
	m_nCntCharge = 0;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CPlayer::~CPlayer()
{
}

//*****************************************************************************
//���[�h����
//*****************************************************************************
HRESULT CPlayer::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "Textures/player.png", &m_pTexture[0]);

	return S_OK;
}

//*****************************************************************************
//�A�����[�h����
//*****************************************************************************
void CPlayer::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCount = 0; nCount < PLAYER_TEXTURE; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CPlayer::Init(void)
{
	//�v���C���[�̏�Ԑݒ�
	m_PlayerState = PLAYER_STATE_NONE;

	if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK ||
		CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
	{
		//�v���C���[�̈ړ����x(�����l)
		m_fPlayerSpeed = PLAYER_MOVE * 1.3f;

		//�v���C���[�̒e�̃`���[�W���x(�����l)
		m_nChargeSpeed = (int)(BULLET_CHARGE_SPEED * 0.6);
	}
	else
	{
		//�v���C���[�̈ړ����x(�����l)
		m_fPlayerSpeed = PLAYER_MOVE;

		//�v���C���[�̒e�̃`���[�W���x(�����l)
		m_nChargeSpeed = BULLET_CHARGE_SPEED;
	}

	//�|�W�V�����̐ݒ�
	SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2), 600.0f, 0.0f));

	//�T�C�Y�̐ݒ�
	SetSize(PLAYER_SIZE);

	//�A�j���[�V�����̏�����
	InitAnimation(ANIMATION_SPEED, ANIMATION_PATTERN, ANIMATION_LINE);

	//�V�[��2D�̏�����
	CScene2D::Init();

	//�e�N�X�`���̊��蓖��
	BindTexture(m_pTexture[0]);

	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(OBJTYPE_PLAYER);

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CPlayer::Uninit(void)
{
	//SE�̒�~
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);

	//�V�[��2D�̏I������
	CScene2D::Uninit();

	if (m_pBullet != NULL)
	{
		m_pBullet = NULL;
	}

	if (m_pEffect != NULL)
	{
		m_pEffect = NULL;
	}

	if (m_pMessage != NULL)
	{
		m_pMessage = NULL;
	}
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CPlayer::Update(void)
{
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	DIJOYSTATE2 Controller = CManager::GetController()->GetController();

	//�ʒu�̎擾
	m_pos = CScene2D::GetPos();

	//��ʊO�ɏo���Ȃ�����
	if (m_pos.x - (PLAYER_SIZE.x / 2) <= WALL_SIZE_X)
	{
		m_pos.x = WALL_SIZE_X + (PLAYER_SIZE.x / 2);
	}
	if (m_pos.x + (PLAYER_SIZE.x / 2) >= SCREEN_WIDTH - WALL_SIZE_X)
	{
		m_pos.x = SCREEN_WIDTH - WALL_SIZE_X - (PLAYER_SIZE.x / 2);
	}

	//�L�[�{�[�h�̓��͏���
	if (pKeyboard->GetKeyboardPress(DIK_A) || Controller.lX < 0)
	{ //A���������Ƃ� �܂��� �X�e�B�b�N�����ɌX�����Ƃ�
		m_pos.x -= m_fPlayerSpeed;

		//�A�j���[�V�����̍X�V����
		CScene2D::UpdateAnimation(ANIMATION_REPEAT, NOT_INVERSION);
	}
	if (pKeyboard->GetKeyboardPress(DIK_D) || Controller.lX > 0)
	{ //D���������Ƃ� �܂��� �X�e�B�b�N���E�ɌX�����Ƃ�
		m_pos.x += m_fPlayerSpeed;

		//�A�j���[�V�����̍X�V����
		CScene2D::UpdateAnimation(ANIMATION_REPEAT, INVERSION);
	}

	if (CManager::GetMode() == CManager::MODE_GAME ||
		CManager::GetMode() == CManager::MODE_SCORE_ATTACK ||
		CManager::GetMode() == CManager::MODE_TUTORIAL_GAME ||
		CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
	{
		if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) || CManager::GetController()->GetJoypadTrigger(2))
		{ //SPACE�L�[ �܂��� A�{�^���������Ă����
			//SE�̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_CHARGE);
		}

		if (pKeyboard->GetKeyboardPress(DIK_SPACE) || CManager::GetController()->GetJoypadPress(2))
		{ //SPACE�L�[ �܂��� A�{�^���������Ă����
			if (m_pBullet == NULL)
			{
				if (m_bRelease == true)
				{
					//�e�̐���
					m_pBullet = CBullet::Create(D3DXVECTOR3(m_pos.x, m_pos.y - (PLAYER_SIZE.y / 2), m_pos.z), m_nCntPress);

					m_bRelease = false;
				}
			}
			if (m_pBullet != NULL)
			{
				//�o���b�g�^�C�v�̐ݒ�
				m_pBullet->SetBulletType(CBullet::BULLET_STATE_NOT_SHOOT);

				//�J���[�̍X�V
				m_pBullet->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.7f));

				//�v���X���Ԃ̉��Z
				m_nCntPress++;

				if (m_nCntPress >= m_nChargeSpeed)
				{ //�}�b�N�X���v���X�����Ƃ�
					//�}�b�N�X�̒l��������
					m_nCntPress = m_nChargeSpeed;

					//�}�b�N�X��ԂŃv���X���Ă��鎞�Ԃ����Z
					m_nCntCharge++;

					if (m_nCntCharge <= 10)
					{ //15�t���[���ȉ��̂Ƃ�
						//�J���[�̍X�V
						m_pBullet->SetColor(D3DXCOLOR(1.0f, 0.3f, 0.3f, 0.7f));
					}
					else if (m_nCntCharge > 10 && m_nCntCharge < 20)
					{ //15�t���[�����v���X�����Ƃ�
						//�J���[�̍X�V
						m_pBullet->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.7f));
					}
					else if (m_nCntCharge == 20)
					{ //30�t���[���v���X�����Ƃ�
						m_nCntCharge = 0;
					}

					//SE�̒�~
					CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);
				}

				//�e�̈ʒu�̍X�V
				m_pBullet->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y - (PLAYER_SIZE.y / 2), m_pos.z));

				//�e�̃T�C�Y�̍X�V
				m_pBullet->SetSize(m_nCntPress);
			}
		}
		if (pKeyboard->GetKeyboardRelease(DIK_SPACE) || CManager::GetController()->GetJoypadRelease(2))
		{ //SPACE�L�[ �܂��� A�{�^���𗣂����Ƃ�
			m_bRelease = true;

			//SE�̒�~
			CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);

			CBullet *pBullet = NULL;

			for (int nCntScene = MAX_OBJECT - 1; nCntScene >= 0; nCntScene--)
			{
				//�I�u�W�F�N�g�̎擾
				CScene *pScene = GetScene(nCntScene);

				//�_�C�i�~�b�N�L���X�g
				pBullet = dynamic_cast<CBullet*>(pScene);

				if (pBullet != NULL)
				{
					break;
				}
			}

			if (pBullet != NULL)
			{
				if (m_pBullet != NULL)
				{
					if (m_nCntPress == m_nChargeSpeed)
					{ //�}�b�N�X�`���[�W��
						//�o���b�g�^�C�v�̐ݒ�
						m_pBullet->SetBulletType(CBullet::BULLET_STATE_MAX);

						//�T�E���h�̍Đ�
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_MAX);
					}
					else
					{
						//�o���b�g�^�C�v�̐ݒ�
						m_pBullet->SetBulletType(CBullet::BULLET_STATE_NONE);

						//�T�E���h�̍Đ�
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_NONE);
					}

					//�J���[�̍X�V
					m_pBullet->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

					//�e�̈ړ��̍X�V
					m_pBullet->SetMove(D3DXVECTOR3(0.0f, BULLET_SPEED_Y, 0.0f));
				}
			}

			if (m_pBullet != NULL)
			{
				//�e�̃|�C���^��������
				m_pBullet = NULL;
			}

			//�e�̃����[�X�J�E���g�̏�����
			m_nCntPress = 0;
		}
	}

	if (m_pBarrier != NULL)
	{
		int nLife = m_pBarrier->GetBarrierLife();

		if (nLife == 0)
		{
			m_pBarrier = NULL;
		}
	}

	//�v���C���[�������Ă���Ƃ�
	if (m_PlayerState == PLAYER_STATE_NONE)
	{
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			//�I�u�W�F�N�g�̎擾
			CScene *pScene = GetScene(nCntScene);

			//�_�C�i�~�b�N�L���X�g
			CEnemy *pEnemy = dynamic_cast<CEnemy*>(pScene);
			CItem *pItem = dynamic_cast<CItem*>(pScene);

			if (pEnemy != NULL)
			{
				//�I�u�W�F�N�g�^�C�v�̎擾
				OBJTYPE objType = pEnemy->GetObjType();

				if (objType == OBJTYPE_BOSS_BALL)
				{ //�t�@�[�X�g�{�[���ɓ��������Ƃ�
					D3DXVECTOR3 targetPos = pEnemy->GetPos();

					if (sqrt(pow(m_pos.x - targetPos.x, 2) + pow((m_pos.y - 30.0f) - targetPos.y, 2)) <= PLAYER_RADIUS + BOSS_BALL_RADIUS)
					{
						m_PlayerState = PLAYER_STATE_DEATH;

						break;
					}
				}
				else if (objType == OBJTYPE_FIRST_BALL)
				{ //�t�@�[�X�g�{�[���ɓ��������Ƃ�
					D3DXVECTOR3 targetPos = pEnemy->GetPos();

					if (sqrt(pow(m_pos.x - targetPos.x, 2) + pow((m_pos.y - 30.0f) - targetPos.y, 2)) <= PLAYER_RADIUS + FIRST_BALL_RADIUS)
					{
						m_PlayerState = PLAYER_STATE_DEATH;

						break;
					}
				}
				else if (objType == OBJTYPE_SECOND_BALL)
				{ //�Z�J���h�{�[���ɓ��������Ƃ�
					D3DXVECTOR3 targetPos = pEnemy->GetPos();

					if (sqrt(pow(m_pos.x - targetPos.x, 2) + pow((m_pos.y - 30.0f) - targetPos.y, 2)) <= PLAYER_RADIUS + SECOND_BALL_RADIUS)
					{
						m_PlayerState = PLAYER_STATE_DEATH;

						break;
					}
				}
				else if (objType == OBJTYPE_THIRD_BALL)
				{ //�T�[�h�{�[���ɓ��������Ƃ�
					D3DXVECTOR3 targetPos = pEnemy->GetPos();

					if (sqrt(pow(m_pos.x - targetPos.x, 2) + pow((m_pos.y - 30.0f) - targetPos.y, 2)) <= PLAYER_RADIUS + THIRD_BALL_RADIUS)
					{
						m_PlayerState = PLAYER_STATE_DEATH;

						break;
					}
				}
			}

			if (pItem != NULL)
			{ //�A�C�e���ɓ��������Ƃ�
				D3DXVECTOR3 targetPos = pItem->GetPos();

				if (sqrt(pow(m_pos.x - targetPos.x, 2) + pow((m_pos.y - 30.0f) - targetPos.y, 2)) <= PLAYER_RADIUS + ITEM_RADIUS)
				{
					//�A�C�e���^�C�v�̎擾
					CItem::ITEMTYPE ItemType = pItem->CItem::GetItemType();

					//�A�C�e���̏I������
					CItem *pItem = (CItem*)pScene;
					pItem->Uninit();

					int nTime = 0;

					//���������A�C�e����
					switch (ItemType)
					{
						//���@�̈ړ��X�s�[�h�A�b�v�������Ƃ�
					case CItem::ITEMTYPE_PLAYER_SPEED_UP:

						//�X�g�b�N�A�C�e���̐ݒ�
						SetItemStock(CItem::ITEMTYPE_PLAYER_SPEED_UP);

						//�G�t�F�N�g�̐���
						CEffect::Create(D3DXVECTOR3(0.0f, -30.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 5.0f, CEffect::EFFECT_TYPE_GET_SPEED_UP, ANIMATION_UNREPEAT, CSound::SOUND_LABEL_SE_HIT1);

						break;

						//�e�̃`���[�W�X�s�[�h�A�b�v�������Ƃ�
					case CItem::ITEMTYPE_CHARGE_SPEED_UP:

						//�X�g�b�N�A�C�e���̐ݒ�
						SetItemStock(CItem::ITEMTYPE_CHARGE_SPEED_UP);

						//�G�t�F�N�g�̐���
						CEffect::Create(D3DXVECTOR3(0.0f, -30.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 5.0f, CEffect::EFFECT_TYPE_GET_TWIN_BULLET, ANIMATION_UNREPEAT, CSound::SOUND_LABEL_SE_HIT1);

						break;

						//�o���A�������Ƃ�
					case CItem::ITEMTYPE_BARRIER:

						//�X�g�b�N�A�C�e���̐ݒ�
						SetItemStock(CItem::ITEMTYPE_BARRIER);

						//�G�t�F�N�g�̐���
						CEffect::Create(D3DXVECTOR3(0.0f, -30.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 5.0f, CEffect::EFFECT_TYPE_GET_BARRIER, ANIMATION_UNREPEAT, CSound::SOUND_LABEL_SE_HIT1);

						break;

						//�^�C�����Z(+1)�������Ƃ�
					case CItem::ITEMTYPE_TIME_EXTENSION_1:

						//�c��^�C���̉��Z
						nTime = CManager::GetTime()->GetTime();
						if (nTime != 0)
						{
							nTime += 1;
							CManager::GetTime()->SetTime(nTime);
						}

						if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
						{
							//�X�R�A�����Z����
							CScoreAttack::AddScore(20);
						}
						else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
						{
							//�X�R�A�����Z����
							CTutorialScoreAttack::AddScore(20);
						}

						//�G�t�F�N�g�̐���
						CEffect::Create(D3DXVECTOR3(0.0f, -30.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 5.0f, CEffect::EFFECT_TYPE_GET_SCORE, ANIMATION_UNREPEAT, CSound::SOUND_LABEL_SE_HIT1);

						break;

						//�^�C�����Z(+5)�������Ƃ�
					case CItem::ITEMTYPE_TIME_EXTENSION_2:

						//�c��^�C���̉��Z
						nTime = CManager::GetTime()->GetTime();
						if (nTime != 0)
						{
							nTime += 5;
							CManager::GetTime()->SetTime(nTime);
						}

						if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
						{
							//�X�R�A�����Z����
							CScoreAttack::AddScore(100);
						}
						else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
						{
							//�X�R�A�����Z����
							CTutorialScoreAttack::AddScore(100);
						}

						//�G�t�F�N�g�̐���
						CEffect::Create(D3DXVECTOR3(0.0f, -30.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 5.0f, CEffect::EFFECT_TYPE_GET_SCORE, ANIMATION_UNREPEAT, CSound::SOUND_LABEL_SE_HIT1);

						break;
					}

					break;
				}
			}
		}
	}

	//�|�W�V�����̐ݒ�
	SetPos(m_pos);

	//�v���C���[�����S���Ă���Ƃ�
	if (m_PlayerState == PLAYER_STATE_DEATH)
	{
		//�����̐���
		CExplosion::Create(m_pos, CSound::SOUND_LABEL_SE_EXPLOSION);

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			//�I�u�W�F�N�g�̎擾
			CScene *pScene = CScene::GetScene(nCntScene);

			//�_�C�i�~�b�N�L���X�g
			CBullet *pBullet = dynamic_cast<CBullet*>(pScene);

			if (pBullet != NULL)
			{
				//�����̐���
				D3DXVECTOR3 BulletPos = pBullet->GetPos();
				CExplosion::Create(BulletPos, CSound::SOUND_LABEL_SE_EXPLOSION);

				//�e�̏I������
				pBullet->Uninit();
			}
		}

		m_pBullet = NULL;
		m_bRelease = true;

		if (m_nLife > 0)
		{//�c�@���c���Ă����Ƃ�
			if (CManager::GetMode() == CManager::MODE_GAME ||
				CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
			{
				//�c�@������炷
				m_nLife--;
			}

			//�v���C���[�̏I������
			Uninit();
			return;
		}
		else if (m_nLife == 0)
		{//�c�@���c���Ă��Ȃ������Ƃ�
			//�c�@������炷
			m_nLife--;

			//���b�Z�[�W�̐���
			if (m_pMessage == NULL)
			{
				m_pMessage = CMessage::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(700.0f, 200.0f, 0.0f), 60, CUi::UI_TYPE_GAME_OVER, CManager::MODE_SELECT);
			}

			//�v���C���[�̃��Z�b�g
			PlayerReset();

			//�v���C���[�̏I������
			Uninit();
			return;
		}
	}

	//�v���C���[�����G��Ԃ̂Ƃ�
	if (m_PlayerState == PLAYER_STATE_INVINCIBLE)
	{
		CBarrier *pBarrier;

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			//�I�u�W�F�N�g�̎擾
			CScene *pScene = GetScene(nCntScene);

			//�_�C�i�~�b�N�L���X�g
			pBarrier = dynamic_cast<CBarrier*>(pScene);

			if (pBarrier != NULL)
			{
				break;
			}
		}

		m_nCntInvincible++;

		if (m_nCntInvincible == 10)
		{
			//�J���[�̍X�V
			if (pBarrier != NULL)
			{
				pBarrier->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}

			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
		if (m_nCntInvincible == 20)
		{
			m_nCntInvincible = 0;

			m_nInvincibleTime++;

			//�J���[�̍X�V
			if (pBarrier != NULL)
			{
				pBarrier->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		//�w��̉񐔓_�ł����Ƃ�
		if (m_nInvincibleTime == 2)
		{
			m_nInvincibleTime = 0;

			//�v���C���[�̏�Ԑݒ�
			m_PlayerState = PLAYER_STATE_NONE;
		}
	}


	//�V�[��2D�̍X�V
	CScene2D::Update();
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CPlayer::Draw(void)
{
	//�V�[��2D�̕`�揈��
	CScene2D::Draw();
}

//*****************************************************************************
//�v���C���[�̃��Z�b�g
//*****************************************************************************
void CPlayer::PlayerReset(void)
{
	//�ړ����x�̃��Z�b�g
	m_fPlayerSpeed = PLAYER_MOVE;

	//�e�̃`���[�W���x�̃��Z�b�g
	m_nChargeSpeed = BULLET_CHARGE_SPEED;

	if (m_pBarrier != NULL)
	{
		if (CManager::GetPlayer()->GetPlayerState() == CPlayer::PLAYER_STATE_NONE)
		{
			//�o���A�̃��Z�b�g
			m_pBarrier->SetBarrierLife(-m_pBarrier->GetBarrierLife());

			m_pBarrier->Uninit();

			m_pBarrier = NULL;
		}
	}
}

//*****************************************************************************
//�A�C�e���̃X�g�b�N����
//*****************************************************************************
void CPlayer:: SetItemStock(CItem::ITEMTYPE ItemType)
{
	//���X�g�̍Ō���ɓ����
	m_StockItem.push_back(ItemType);

	if (m_StockItem.size() > 3)
	{ //���X�g�̗v�f�����R�𒴂���Ƃ�
		//���X�g�̐擪�v�f������ (.erase(�w��̗v�f) : �w��̗v�f������) (begin() : �擪�̗v�f)
		m_StockItem.erase(m_StockItem.begin());
	}

	//�v���C���[�̃A�C�e�����ʂ̃��Z�b�g
	PlayerReset();

	//�v���C���[�̃A�C�e�����ʂ̍X�V
	SetPlayerAbility();

	//�A�C�e���X�g�b�NUI�̍X�V����
	CGame::UpdateStock(m_StockItem);
}

//*****************************************************************************
//�v���C���[�̃A�C�e�����ʂ̍X�V����
//*****************************************************************************
void CPlayer::SetPlayerAbility(void)
{
	for (const auto &count : m_StockItem)
	{
		switch (count)
		{
			//���@�̈ړ��X�s�[�h�A�b�v�������Ƃ�
		case CItem::ITEMTYPE_PLAYER_SPEED_UP:

			//�ړ��X�s�[�h���グ��
			m_fPlayerSpeed += PLAYER_SPEED_UP;

			break;

			//�e�̃`���[�W�X�s�[�h�A�b�v�������Ƃ�
		case CItem::ITEMTYPE_CHARGE_SPEED_UP:

			//�e�̃`���[�W�X�s�[�h������������
			m_nChargeSpeed += BULLET_CHARGE_SPEED_DECREASE;

			break;

			//�o���A�������Ƃ�
		case CItem::ITEMTYPE_BARRIER:

			if (m_pBarrier == NULL)
			{
				//�o���A�̐��� 
				m_pBarrier = CBarrier::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));
			}
			else if (m_pBarrier != NULL)
			{
				if (m_pBarrier->GetBarrierLife() == 0)
				{
					m_pBarrier = NULL;

					//�o���A�̐��� 
					m_pBarrier = CBarrier::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));
				}
				else
				{
					//�o���A�̃��C�t�ݒ�
					m_pBarrier->SetBarrierLife(1);
				}
			}
		}
	}
}

//*****************************************************************************
//�o���A���ʂ��P��������
//*****************************************************************************
void CPlayer::DeleteBarrierAbility(void)
{
	//�v���C���[�̃A�C�e�����ʂ̃��Z�b�g
	PlayerReset();

	for (auto itr = m_StockItem.begin(); itr != m_StockItem.end(); itr++)
	{
		if (*itr == CItem::ITEMTYPE_BARRIER)
		{
			//�w��̃��X�g������
			m_StockItem.erase(itr);

			break;
		}
	}

	//�v���C���[�̃A�C�e�����ʂ̍X�V
	SetPlayerAbility();

	//�A�C�e���X�g�b�NUI�̍X�V����
	CGame::UpdateStock(m_StockItem);
}

//*****************************************************************************
//�v���C���[�̃��C�t�̐ݒ�
//*****************************************************************************
void CPlayer::SetLife(int nLife)
{
	m_nLife = nLife;
}

//*****************************************************************************
//�v���C���[�̏�Ԃ̐ݒ�
//*****************************************************************************
void CPlayer::SetPlayerState(PLAYER_STATE state)
{
	m_PlayerState = state;
}

//*****************************************************************************
//�v���X�J�E���g�̐ݒ�
//*****************************************************************************
void CPlayer::SetPressCount(int num)
{
	m_nCntPress = num;

	m_bRelease = true;
}

//*****************************************************************************
//�o���b�g�|�C���^��NULL�ɂ���
//*****************************************************************************
void CPlayer::SetBulletPointerNULL(void)
{
	m_pBullet = NULL;
}

//*****************************************************************************
//�|�W�V�����̎擾
//*****************************************************************************
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}

//*****************************************************************************
//�o���A�̎擾
//*****************************************************************************
CBarrier *CPlayer::GetBarrier(void)
{
	return m_pBarrier;
}

//*****************************************************************************
//�o���b�g�̎擾
//*****************************************************************************
CBullet *CPlayer::GetBullet(void)
{
	return m_pBullet;
}

//*****************************************************************************
//�v���C���[�̎c�@�̎擾
//*****************************************************************************
int CPlayer::GetPlayerLife(void)
{
	return m_nLife;
}

//*****************************************************************************
//�e�̃`���[�W�X�s�[�h�̎擾
//*****************************************************************************
int CPlayer::GetChargeSpeed(void)
{
	return m_nChargeSpeed;
}

//*****************************************************************************
//�v���C���[�̎c�@�̎擾
//*****************************************************************************
list<CItem::ITEMTYPE> CPlayer::GetStockItem(void)
{
	return m_StockItem;
}

//*****************************************************************************
//�v���C���[�̏�Ԃ̎擾
//*****************************************************************************
CPlayer::PLAYER_STATE CPlayer::GetPlayerState(void)
{
	return m_PlayerState;
}