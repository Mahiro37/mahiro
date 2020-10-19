//=============================================================================
//
// �`���[�g���A������(�X�R�A�A�^�b�N���[�h) [tutorial_score_attack.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "tutorial_score_attack.h"
#include "manager.h"
#include "keyboard.h"
#include "controller.h"
#include "bg.h"
#include "enemy.h"
#include "bullet.h"
#include "fade.h"
#include "door.h"
#include "time.h"
#include "ui.h"
#include "score.h"
#include "player.h"
#include "scene.h"
#include "explosion.h"

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
CScore *CTutorialScoreAttack::m_pScore = NULL;

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CTutorialScoreAttack::CTutorialScoreAttack()
{
	m_pDescriptionUi = NULL;
	m_bBall = false;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CTutorialScoreAttack::~CTutorialScoreAttack()
{
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CTutorialScoreAttack::Init(void)
{
	//BGM�̍Đ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM06);

	//�`���[�g���A�����[�h�̐ݒ�
	SetTutorialMode(TUTORIAL_MODE_SCORE_ATTACK);

	//�w�i�̐���
	CBg::Create();

	//�v���C���[�̐���
	CManager::CreatePlayer();

	//�X�R�A�A�^�b�N���[�hUI�̐���
	CUi::Create(D3DXVECTOR3(320.0f, 105.0f, 0.0f), D3DXVECTOR3(350.0f, 150.0f, 0.0f), CUi::UI_TYPE_TUTORIAL_SCORE_ATTACK);

	//�^�C���̐���
	CManager::CreateTime(D3DXVECTOR3((SCREEN_WIDTH / 2) + 20.0f, 80.0f, 0.0f), D3DXVECTOR3(40.0f, 50.0f, 0.0f), TIME_START_NUM, CNumber::TYPE_2);
	CManager::GetTime()->SetStopTime(false);

	//�^�C�}�[UI�̐���
	CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 70.0f, 0.0f), D3DXVECTOR3(110.0f, 130.0f, 0.0f), CUi::UI_TYPE_TIMER);

	//�h�A�̐���
	CDoor::Create(D3DXVECTOR3(229.8f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_SELECT);

	//�X�R�A�̐���
	if (m_pScore == NULL)
	{
		m_pScore = CScore::Create(D3DXVECTOR3(1070.0f, 80.0f, 0.0f), D3DXVECTOR3(50.0f, 60.0f, 0.0f), 30.0f, 0, CNumber::TYPE_2);
	}

	//�X�R�AUI�̐���
	CUi::Create(D3DXVECTOR3(880.0f, 80.0f, 0.0f), D3DXVECTOR3(90.0f, 50.0f, 0.0f), CUi::UI_TYPE_SCORE);

	//�{�[���̐����ʒu��UI�̐���
	CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 210.0f, 0.0f), D3DXVECTOR3(120.0f, 120.0f, 0.0f), CUi::UI_TYPE_BALL_CREATE);

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CTutorialScoreAttack::Uninit(void)
{
	//BGM�̒�~
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM06);

	if (m_pScore != NULL)
	{
		m_pScore->SetScore(0);
		m_pScore->Uninit();
		m_pScore = NULL;
	}

	//�S�Ẵ������̊J������
	CScene::ReleaseAllOtherThanFadeAndPlayer();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CTutorialScoreAttack::Update(void)
{
	if (CScene::GetUpdateStop() == false)
	{
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			//�I�u�W�F�N�g�̎擾
			CScene *pScene = CScene::GetScene(nCntScene);

			//�_�C�i�~�b�N�L���X�g
			CEnemy *pEnemy = dynamic_cast<CEnemy*>(pScene);

			if (pEnemy != NULL)
			{
				m_bBall = true;

				break;
			}
			else
			{
				m_bBall = false;
			}
		}

		bool bPlayer = false;

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			//�I�u�W�F�N�g�̎擾
			CScene *pScene = CScene::GetScene(nCntScene);

			//�_�C�i�~�b�N�L���X�g
			CPlayer *pPlayer = dynamic_cast<CPlayer*>(pScene);

			if (pPlayer != NULL)
			{
				bPlayer = true;

				break;
			}
		}

		CKeyboard *pKeyboard = CManager::GetKeyboard();
		int nTime = CManager::GetTime()->GetTime();

		if (pKeyboard->GetKeyboardTrigger(DIK_LEFT) || pKeyboard->GetKeyboardTrigger(DIK_RIGHT) || CManager::GetController()->GetJoypadTrigger(4) || CManager::GetController()->GetJoypadTrigger(5))
		{ //����� �܂��� �E��� �܂��� L1 �܂��� R1���������Ƃ�
			//�`���[�g���A�����[�h�̐ݒ�
			SetTutorialMode(TUTORIAL_MODE_GAME);

			//�t�F�[�h�̐���
			CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_TUTORIAL_GAME);
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_R) || CManager::GetController()->GetJoypadTrigger(1) || nTime == 0 || bPlayer == false)
		{ //R�L�[ �܂��� Y�{�^�����������Ƃ� �܂��� �^�C����0�ɂȂ����Ƃ� �܂��� �v���C���[�����񂾂Ƃ�
			//SE�̒�~
			CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);

			//�`���[�g���A�����[�h�̐ݒ�
			SetTutorialMode(TUTORIAL_MODE_SCORE_ATTACK);

			//�t�F�[�h�̐���
			CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_TUTORIAL_SCORE_ATTACK);
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_B) || CManager::GetController()->GetJoypadTrigger(6))
		{ //B�L�[ �܂��� L2���������Ƃ�
			if (m_bBall == false)
			{
				CEnemy::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 210.0f, 0.0f), THIRD_BALL_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CScene::OBJTYPE_THIRD_BALL);
			}
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_N) || CManager::GetController()->GetJoypadTrigger(7))
		{ //N�L�[ �܂��� R2���������Ƃ�
			if (m_bBall == false)
			{
				CEnemy::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 210.0f, 0.0f), SECOND_BALL_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CScene::OBJTYPE_SECOND_BALL);
			}
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_X) || CManager::GetController()->GetJoypadTrigger(0))
		{ //X�L�[ �܂��� X�{�^�����������Ƃ�
			if (m_pDescriptionUi == NULL)
			{
				m_pDescriptionUi = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CUi::UI_TYPE_DESCRIPTION);

				//SE�̒�~
				CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);
				CManager::GetSound()->Stop(CSound::SOUND_LABEL_DOOR_OPEN);

				//�v���X�J�E���g�̏�����
				CManager::GetPlayer()->SetPressCount(0);

				for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
				{
					//�I�u�W�F�N�g�̎擾
					CScene *pScene = CScene::GetScene(nCntScene);

					//�_�C�i�~�b�N�L���X�g
					CBullet *pBullet = dynamic_cast<CBullet*>(pScene);

					if (pBullet != NULL)
					{
						//�J���[�̍X�V
						pBullet->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						//�e�̈ړ��̍X�V
						pBullet->SetMove(D3DXVECTOR3(0.0f, BULLET_SPEED_Y, 0.0f));

						//�o���b�g�|�C���^��NULL�ɂ���
						CManager::GetPlayer()->SetBulletPointerNULL();
					}
				}

				CScene::UpdateStop(true);
			}
		}
	}
	else
	{
		if (m_pDescriptionUi != NULL)
		{
			if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_X) || CManager::GetController()->GetJoypadTrigger(0))
			{ //X�L�[ �܂��� X�{�^�����������Ƃ�
				m_pDescriptionUi->Uninit();
				m_pDescriptionUi = NULL;

				//SE�̍Đ�
				CManager::GetSound()->Play(CSound::SOUND_LABEL_PAPER_CLOSE);

				CScene::UpdateStop(false);
			}
		}
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CTutorialScoreAttack::Draw(void)
{
}

//*****************************************************************************
//�X�R�A�̉��Z
//*****************************************************************************
void CTutorialScoreAttack::AddScore(int nNum)
{
	if (m_pScore != NULL)
	{
		m_pScore->AddScore(nNum);
	}
}