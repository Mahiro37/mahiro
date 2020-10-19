//=============================================================================
//
// �X�R�A�A�^�b�N���� [score_attack.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include <time.h>
#include "score_attack.h"
#include "manager.h"
#include "keyboard.h"
#include "time.h"
#include "enemy.h"
#include "bg.h"
#include "player.h"
#include "score.h"
#include "life.h"
#include "sound.h"
#include "scene.h"
#include "ui.h"
#include "fade.h"
#include "ranking.h"
#include "message.h"

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
CScore *CScoreAttack::m_pScore = NULL;

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CScoreAttack::CScoreAttack()
{
	m_pMessage = NULL;
	m_pFade = NULL;
	m_nTime = TIME_START_NUM;
	m_nCntResult = 0;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CScoreAttack::~CScoreAttack()
{
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CScoreAttack::Init(void)
{
	//�w�i�̐���
	CBg::Create();

	//�t�@�[�X�g�{�[���̐���
	CEnemy::Create(D3DXVECTOR3(800.0f, 200.0f, 0.0f), FIRST_BALL_SIZE, D3DXVECTOR3(3.0f, -2.0f, 0.0f), CScene::OBJTYPE_FIRST_BALL);
	CEnemy::Create(D3DXVECTOR3(400.0f, 200.0f, 0.0f), FIRST_BALL_SIZE, D3DXVECTOR3(-3.0f, -2.0f, 0.0f), CScene::OBJTYPE_FIRST_BALL);

	//�v���C���[�̐���
	CManager::CreatePlayer();

	//�^�C���̐���
	CManager::CreateTime(D3DXVECTOR3((SCREEN_WIDTH / 2) + 20.0f, 80.0f, 0.0f), D3DXVECTOR3(40.0f, 50.0f, 0.0f), m_nTime, CNumber::TYPE_2);
	CManager::GetTime()->SetTime(CManager::GetTime()->GetSaveTime());
	CManager::GetTime()->SetStopTime(false);

	//�^�C�}�[UI�̐���
	CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 70.0f, 0.0f), D3DXVECTOR3(110.0f, 130.0f, 0.0f), CUi::UI_TYPE_TIMER);

	//�c�@�̐���
	CManager::CreateLife(D3DXVECTOR3(350.0f, 80.0f, 0.0f));
	int nLife = CManager::GetPlayer()->GetPlayerLife();
	CManager::GetLife()->SetLife(nLife);

	//�c�@UI�̐���
	CUi::Create(D3DXVECTOR3(260.0f, 80.0f, 0.0f), D3DXVECTOR3(130.0f, 60.0f, 0.0f), CUi::UI_TYPE_ZANKI);

	//�X�R�A�̐���
	if (m_pScore == NULL)
	{
		m_pScore = CScore::Create(D3DXVECTOR3(1070.0f, 80.0f, 0.0f), D3DXVECTOR3(50.0f, 60.0f, 0.0f), 30.0f, 0, CNumber::TYPE_2);
	}
	if (m_pScore != NULL)
	{
		m_pScore->SetScore(m_pScore->GetScore());
	}

	//�X�R�AUI�̐���
	CUi::Create(D3DXVECTOR3(880.0f, 80.0f, 0.0f), D3DXVECTOR3(90.0f, 50.0f, 0.0f), CUi::UI_TYPE_SCORE);

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CScoreAttack::Uninit(void)
{
	//BGM�̒�~
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM05);

	//�S�Ẵ������̊J������
	CScene::ReleaseAllOtherThanFadeAndPlayer();

	if (m_pScore != NULL)
	{
		m_pScore->Uninit();
		m_pScore = NULL;
	}

	if (m_pMessage != NULL)
	{
		m_pMessage = NULL;
	}

	if (m_pFade != NULL)
	{
		m_pFade = NULL;
	}
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CScoreAttack::Update(void)
{
	int nTimeNow = 0;

	//���Ԃ̎擾
	if (CManager::GetTime() != NULL)
	{
		nTimeNow = CManager::GetTime()->GetTime();
	}

	if (nTimeNow != 0)
	{
		bool bBall = false;

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			//�I�u�W�F�N�g�̎擾
			CScene *pScene = CScene::GetScene(nCntScene);

			//�_�C�i�~�b�N�L���X�g
			CEnemy *pEnemy = dynamic_cast<CEnemy*>(pScene);

			if (pEnemy != NULL)
			{
				bBall = true;
			}
		}

		if (bBall == false)
		{
			//�t�@�[�X�g�{�[���̐���
			CEnemy::Create(D3DXVECTOR3(640.0f, -100.0f, 0.0f), FIRST_BALL_SIZE, D3DXVECTOR3(3.0f, -2.0f, 0.0f), CScene::OBJTYPE_FIRST_BALL);
			CEnemy::Create(D3DXVECTOR3(640.0f, -100.0f, 0.0f), FIRST_BALL_SIZE, D3DXVECTOR3(-3.0f, -2.0f, 0.0f), CScene::OBJTYPE_FIRST_BALL);
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
			}
		}

		if (bPlayer == false)
		{
			if (CManager::GetTime() != NULL)
			{
				//���Ԃ��~�߂�
				CManager::GetTime()->SetStopTime(true);

				//���Ԃ̕ۊ�
				CManager::GetTime()->SaveTime(nTimeNow);
			}

			if (CManager::GetPlayer()->GetPlayerLife() >= 0)
			{
				if (m_pFade == NULL)
				{
					//�t�F�[�h�̐���
					m_pFade = CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_SCORE_ATTACK);
				}
			}
		}
	}
	else
	{
		//SE�̒�~
		CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);

		if (m_pMessage == NULL)
		{
			//���b�Z�[�W�̐���
			m_pMessage = CMessage::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(400.0f, 200.0f, 0.0f), 60, CUi::UI_TYPE_FINISH, CManager::MODE_SCORE_SCREEN);
		}
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CScoreAttack::Draw(void)
{
}

//*****************************************************************************
//�X�R�A�̉��Z����
//*****************************************************************************
void CScoreAttack::AddScore(int nNum)
{
	m_pScore->AddScore(nNum);
}

//*****************************************************************************
//�X�R�A�̎擾
//*****************************************************************************
CScore *CScoreAttack::GetScore(void)
{
	return m_pScore;
}