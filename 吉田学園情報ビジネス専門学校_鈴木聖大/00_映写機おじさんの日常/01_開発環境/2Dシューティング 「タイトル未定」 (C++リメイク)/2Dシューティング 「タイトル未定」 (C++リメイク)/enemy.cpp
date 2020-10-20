//=============================================================================
//
// �G���� [enemy.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "barrier.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define BALL_GRAVITY		(0.15f)		//�{�[���̏d��
#define BALL_ELASTICITY		(11.0f)		//�{�[���̒e��
#define BOSS_BALL_LIFE		(15)		//�{�X�{�[���̃��C�t
#define FIRST_BALL_LIFE		(5)			//�t�@�[�X�g�{�[���̃��C�t
#define SECOND_BALL_LIFE	(3)			//�Z�J���h�{�[���̃��C�t
#define THIRD_BALL_LIFE		(2)			//�T�[�h�{�[���̃��C�t

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[TYPE_MAX] = {};

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CEnemy::CEnemy()
{
	m_EnemyState = STATE_NONE;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BarrierPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nBarrierLife = 0;
	m_nBallLife = 0;
	m_nDamageColorChange = 0;
	m_nInvincibleTime = 0;
	m_nCntCreateBall = 0;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CEnemy::~CEnemy()
{
}

//*****************************************************************************
//���[�h����
//*****************************************************************************
HRESULT CEnemy::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "Textures/ball/ball00.png", &m_pTexture[TYPE_FIRST]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ball/ball01.png", &m_pTexture[TYPE_SECOND]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ball/ball02.png", &m_pTexture[TYPE_THIRD]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ball/ball03.png", &m_pTexture[TYPE_BOSS]);

	return S_OK;
}

//*****************************************************************************
//�A�����[�h����
//*****************************************************************************
void CEnemy::Unload(void)
{
	//�e�N�X�`���̔j��
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//�G�̐���
//*****************************************************************************
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, OBJTYPE type)
{
	//�G�l�~�[�̃������m��
	CEnemy *pEnemy = new CEnemy;

	if (pEnemy != NULL)
	{
		//�G�l�~�[�̏���������
		pEnemy->Init(pos, size, move, type);
	}

	return pEnemy;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, OBJTYPE type)
{
	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(type);

	//�G�l�~�[�̐ݒ�
	SetEnemy(pos, move);

	//�T�C�Y�̐ݒ�
	SetSize(size);

	switch (type)
	{
	case OBJTYPE_FIRST_BALL:

		//�e�N�X�`���̊��蓖��
		BindTexture(m_pTexture[TYPE_FIRST]);

		m_nBallLife = FIRST_BALL_LIFE;

		break;

	case OBJTYPE_SECOND_BALL:

		//�e�N�X�`���̊��蓖��
		BindTexture(m_pTexture[TYPE_SECOND]);

		m_nBallLife = SECOND_BALL_LIFE;

		break;

	case OBJTYPE_THIRD_BALL:

		//�e�N�X�`���̊��蓖��
		BindTexture(m_pTexture[TYPE_THIRD]);

		m_nBallLife = THIRD_BALL_LIFE;

		break;

	case OBJTYPE_BOSS_BALL:

		//�e�N�X�`���̊��蓖��
		BindTexture(m_pTexture[TYPE_BOSS]);

		m_nBallLife = BOSS_BALL_LIFE;

		break;

	default:

		break;
	}

	//�V�[��2D�̏�����
	CScene2D::Init();

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CEnemy::Uninit(void)
{
	//�V�[��2D�̏I������
	CScene2D::Uninit();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CEnemy::Update(void)
{
	//�V�[��2D�̍X�V����
	CScene2D::Update();

	D3DXVECTOR3 pos = GetPos();

	//�d�͂����Z
	m_move.y += BALL_GRAVITY;

	//�ړ�����
	pos += m_move;

	//�{�[���^�C�v�̎擾
	OBJTYPE BallType = GetObjType();

	switch (BallType)
	{
	case OBJTYPE_BOSS_BALL:

		//���ˏ���
		Reflection(pos, BOSS_BALL_SIZE);

		//��]����
		RotationEnemy(pos, BOSS_BALL_SIZE, m_move);

		//�G���𐶐�����J�E���g��i�߂�
		m_nCntCreateBall++;

		if (m_nCntCreateBall == 480)
		{ //180�t���[���o�����Ƃ�
			//�T�[�h�{�[���̐���
			CEnemy::Create(D3DXVECTOR3(pos.x, pos.y - (BOSS_BALL_SIZE.y / 2) - (THIRD_BALL_SIZE.y / 2), 0.0f), THIRD_BALL_SIZE, D3DXVECTOR3(0.0f, -5.0f, 0.0f), OBJTYPE_THIRD_BALL);
		}
		else if (m_nCntCreateBall == 495)
		{ //195�t���[���o�����Ƃ�
			//�T�[�h�{�[���̐���
			CEnemy::Create(D3DXVECTOR3(pos.x, pos.y - (BOSS_BALL_SIZE.y / 2) - (THIRD_BALL_SIZE.y / 2), 0.0f), THIRD_BALL_SIZE, D3DXVECTOR3(0.0f, -5.0f, 0.0f), OBJTYPE_THIRD_BALL);
		}
		else if (m_nCntCreateBall == 510)
		{ //210�t���[���o�����Ƃ�
			//�J�E���g�̏�����
			m_nCntCreateBall = 0;

		  //�T�[�h�{�[���̐���
			CEnemy::Create(D3DXVECTOR3(pos.x, pos.y - (BOSS_BALL_SIZE.y / 2) - (THIRD_BALL_SIZE.y / 2), 0.0f), THIRD_BALL_SIZE, D3DXVECTOR3(0.0f, -5.0f, 0.0f), OBJTYPE_THIRD_BALL);
		}

		break;

	case OBJTYPE_FIRST_BALL:

		//���ˏ���
		Reflection(pos, FIRST_BALL_SIZE);

		//��]����
		RotationEnemy(pos, FIRST_BALL_SIZE, m_move);

		break;

	case OBJTYPE_SECOND_BALL:

		//���ˏ���
		Reflection(pos,SECOND_BALL_SIZE);

		//��]����
		RotationEnemy(pos,SECOND_BALL_SIZE, m_move);

		break;

	case OBJTYPE_THIRD_BALL:

		//���ˏ���
		Reflection(pos, THIRD_BALL_SIZE);

		//��]����
		RotationEnemy(pos,  THIRD_BALL_SIZE, m_move);

		break;

	default:

		break;
	}

	//�|�W�V�����̐ݒ�
	SetPos(pos);

	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		//�I�u�W�F�N�g�̎擾
		CScene *pScene = GetScene(nCntScene);

		CBarrier *pBarrier = dynamic_cast<CBarrier*>(pScene);
		CBullet *pBullet = dynamic_cast<CBullet*>(pScene);

		if (pBarrier != NULL)
		{
			D3DXVECTOR3 BarrierPos = pBarrier->GetPos();

			switch (BallType)
			{
			case OBJTYPE_BOSS_BALL:

				if (sqrt(pow(pos.x - BarrierPos.x, 2) + pow(pos.y - BarrierPos.y, 2)) <= BOSS_BALL_RADIUS + BARRIER_RADIUS)
				{
					//SE�̍Đ�
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT1);

					//���˂�����
					if (m_move.y > 0.0f)
					{
						if (m_move.x >= 0.0f)
						{
							if (pos.x >= BarrierPos.x)
							{
								m_move.y = m_move.y * -1.0f;
							}
							else
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
						}
						else
						{
							if (pos.x > BarrierPos.x)
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
							else
							{
								m_move.y = m_move.y * -1.0f;
							}
						}
					}
					else
					{
						m_move.x = m_move.x * -1.0f;
					}

					if (CManager::GetPlayer() != NULL)
					{
						if (CManager::GetPlayer()->GetPlayerState() == CPlayer::PLAYER_STATE_NONE)
						{
							//�o���AUI�̏���
							CManager::GetPlayer()->DeleteBarrierAbility();

							//�v���C���[�̏�Ԃ̐ݒ�
							CManager::GetPlayer()->SetPlayerState(CPlayer::PLAYER_STATE_INVINCIBLE);
						}
					}
				}

				break;

			case OBJTYPE_FIRST_BALL:

				if (sqrt(pow(pos.x - BarrierPos.x, 2) + pow(pos.y - BarrierPos.y, 2)) <= FIRST_BALL_RADIUS + BARRIER_RADIUS)
				{
					//SE�̍Đ�
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT1);

					//���˂�����
					if (m_move.y > 0.0f)
					{
						if (m_move.x >= 0.0f)
						{
							if (pos.x >= BarrierPos.x)
							{
								m_move.y = m_move.y * -1.0f;
							}
							else
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
						}
						else
						{
							if (pos.x > BarrierPos.x)
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
							else
							{
								m_move.y = m_move.y * -1.0f;
							}
						}
					}
					else
					{
						m_move.x = m_move.x * -1.0f;
					}

					if (CManager::GetPlayer() != NULL)
					{
						if (CManager::GetPlayer()->GetPlayerState() == CPlayer::PLAYER_STATE_NONE)
						{
							//�o���AUI�̏���
							CManager::GetPlayer()->DeleteBarrierAbility();

							//�v���C���[�̏�Ԃ̐ݒ�
							CManager::GetPlayer()->SetPlayerState(CPlayer::PLAYER_STATE_INVINCIBLE);
						}
					}
				}

				break;

			case OBJTYPE_SECOND_BALL:

				if (sqrt(pow(pos.x - BarrierPos.x, 2) + pow(pos.y - BarrierPos.y, 2)) <= SECOND_BALL_RADIUS + BARRIER_RADIUS)
				{
					//SE�̍Đ�
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT1);

					//���˂�����
					if (m_move.y > 0.0f)
					{
						if (m_move.x >= 0.0f)
						{
							if (pos.x >= BarrierPos.x)
							{
								m_move.y = m_move.y * -1.0f;
							}
							else
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
						}
						else
						{
							if (pos.x > BarrierPos.x)
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
							else
							{
								m_move.y = m_move.y * -1.0f;
							}
						}
					}
					else
					{
						m_move.x = m_move.x * -1.0f;
					}

					if (CManager::GetPlayer() != NULL)
					{
						if (CManager::GetPlayer()->GetPlayerState() == CPlayer::PLAYER_STATE_NONE)
						{
							//�o���AUI�̏���
							CManager::GetPlayer()->DeleteBarrierAbility();

							//�v���C���[�̏�Ԃ̐ݒ�
							CManager::GetPlayer()->SetPlayerState(CPlayer::PLAYER_STATE_INVINCIBLE);
						}
					}
				}

				break;

			case OBJTYPE_THIRD_BALL:

				if (sqrt(pow(pos.x - BarrierPos.x, 2) + pow(pos.y - BarrierPos.y, 2)) <= THIRD_BALL_RADIUS + BARRIER_RADIUS)
				{
					//SE�̍Đ�
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT1);

					//���˂�����
					if (m_move.y > 0.0f)
					{
						if (m_move.x >= 0.0f)
						{
							if (pos.x > BarrierPos.x)
							{
								m_move.y = m_move.y * -1.0f;
							}
							else
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
						}
						else
						{
							if (pos.x > BarrierPos.x)
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
							else
							{
								m_move.y = m_move.y * -1.0f;
							}
						}
					}
					else
					{
						if (BarrierPos.x - m_BarrierPosOld.x < 0.0f)
						{
							if (m_move.x >= 0.0f)
							{
								m_move.x = m_move.x * -1.0f;
							}
						}
						else if (BarrierPos.x - m_BarrierPosOld.x >= 0.0f)
						{
							if (m_move.x < 0.0f)
							{
								m_move.x = m_move.x * -1.0f;
							}
						}
					}

					if (CManager::GetPlayer() != NULL)
					{
						if (CManager::GetPlayer()->GetPlayerState() == CPlayer::PLAYER_STATE_NONE)
						{
							//�o���AUI�̏���
							CManager::GetPlayer()->DeleteBarrierAbility();

							//�v���C���[�̏�Ԃ̐ݒ�
							CManager::GetPlayer()->SetPlayerState(CPlayer::PLAYER_STATE_INVINCIBLE);
						}
					}
				}

				break;

			default:

				break;
			}

			if (pBarrier != NULL)
			{
				//�o���A�̃��C�t�̎擾
				m_nBarrierLife = pBarrier->GetBarrierLife();

				//�o���A���C�t���O�ɂȂ����Ƃ�
				if (m_nBarrierLife == 0)
				{
					pBarrier = NULL;
				}
			}

			m_BarrierPosOld = BarrierPos;
		}
	}

	if (m_EnemyState == STATE_DAMAGE)
	{
		//�J���[�̍X�V
		SetColor(D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f));

		m_nDamageColorChange++;

		if (m_nDamageColorChange > 3)
		{
			m_nDamageColorChange = 0;

			m_EnemyState = STATE_NONE;

			if (m_EnemyState == STATE_NONE)
			{
				//�J���[�̍X�V
				SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CEnemy::Draw(void)
{
	//�V�[��2D�̕`�揈��
	CScene2D::Draw();
}

//*****************************************************************************
//�ݒ菈��
//*****************************************************************************
void CEnemy::SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	m_move = move;

	SetPos(pos);
}

//*****************************************************************************
//�{�[���̏�Ԃ̐ݒ�
//*****************************************************************************
void CEnemy::SetBallState(ENEMY_STATE state)
{
	m_EnemyState = state;
}

//*****************************************************************************
//�ݒ菈��
//*****************************************************************************
void CEnemy::SetBallLife(int nDamage)
{
	m_nBallLife -= nDamage;
}

//*****************************************************************************
//���ˏ���
//*****************************************************************************
void CEnemy::Reflection(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//�{�[���^�C�v�̎擾
	OBJTYPE BallType = GetObjType();

	//���̕ǂɓ��������Ƃ�
	if ((pos.x - (size.x / 2.0f)) < WALL_SIZE_X)
	{
		//�ǂ̊O�ɏo����߂�
		pos.x = WALL_SIZE_X + size.x;

		//���˂�����
		if (m_move.x < 0.0f)
		{
			m_move.x = m_move.x * -1.0f;
		}

		switch (BallType)
		{
		case OBJTYPE_BOSS_BALL:

			//�o�E���h���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BOSS_BALL);

			break;

		case OBJTYPE_FIRST_BALL:

			//�o�E���h���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_FIRST_BALL);

			break;

		case OBJTYPE_SECOND_BALL:

			//�o�E���h���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SECOND_BALL);

			break;

		case OBJTYPE_THIRD_BALL:

			//�o�E���h���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_THIRD_BALL);

			break;

		default:

			break;
		}
	}

	//�E�̕ǂɓ��������Ƃ�
	if ((pos.x + (size.x / 2.0f)) > SCREEN_WIDTH - WALL_SIZE_X)
	{
		//�ǂ̊O�ɏo����߂�
		pos.x = (SCREEN_WIDTH - WALL_SIZE_X) - (size.x / 2);

		//���˂�����
		if (m_move.x > 0.0f)
		{
			m_move.x = m_move.x * -1.0f;
		}

		switch (BallType)
		{
		case OBJTYPE_BOSS_BALL:

			//�o�E���h���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BOSS_BALL);

			break;

		case OBJTYPE_FIRST_BALL:

			//�o�E���h���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_FIRST_BALL);

			break;

		case OBJTYPE_SECOND_BALL:

			//�o�E���h���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SECOND_BALL);

			break;

		case OBJTYPE_THIRD_BALL:

			//�o�E���h���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_THIRD_BALL);

			break;

		default:

			break;
		}
	}

	//���ɓ��������Ƃ�
	if ((pos.y + (size.y / 2.0f)) >= SCREEN_HEIGHT - FLOOR_SIZE_Y)
	{
		//���˂�����
		m_move.y = BALL_ELASTICITY * -1.0f;

		switch (BallType)
		{
		case OBJTYPE_BOSS_BALL:

			//�o�E���h���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BOSS_BALL);

			break;

		case OBJTYPE_FIRST_BALL:

			//�o�E���h���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_FIRST_BALL);

			break;

		case OBJTYPE_SECOND_BALL:

			//�o�E���h���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SECOND_BALL);

			break;

		case OBJTYPE_THIRD_BALL:

			//�o�E���h���̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_THIRD_BALL);

			break;

		default:

			break;
		}
	}
}

//*****************************************************************************
//��]����
//*****************************************************************************
void CEnemy::RotationEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move)
{
	if (move.x > 0.0f)
	{ //�E�Ɉړ����Ă���Ƃ�
		//��]����
		Rotation(pos, size, ROTATION_RIGHT);
	}
	else if (move.x < 0.0f)
	{ //���Ɉړ����Ă���Ƃ�
		//��]����
		Rotation(pos, size, ROTATION_LEFT);
	}
}

//*****************************************************************************
//�{�[���̃��C�t�̎擾
//*****************************************************************************
int CEnemy::GetBallLife(void)
{
	return m_nBallLife;
}