//=============================================================================
//
// �}�l�W�����g���� [manager.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "keyboard.h"
#include "controller.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2d.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "sound.h"
#include "bg.h"
#include "enemy.h"
#include "item.h"
#include "effect.h"
#include "score.h"
#include "life.h"
#include "barrier.h"
#include "time.h"
#include "title.h"
#include "game.h"
#include "score_attack.h"
#include "select.h"
#include "tutorial.h"
#include "tutorial_game.h"
#include "tutorial_score_attack.h"
#include "GameClear.h"
#include "ScoreScreen.h"
#include "ranking.h"
#include "ui.h"
#include "fade.h"
#include "door.h"

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
CRenderer		*CManager::m_pRenderer	=	NULL;
CKeyboard		*CManager::m_pKeyboard	=	NULL;
CController		*CManager::m_pController=	NULL;
CSound			*CManager::m_pSound		=	NULL;
CMode			*CManager::m_pMode		=	NULL;
CTime			*CManager::m_pTime		=	NULL;
CLife			*CManager::m_pLife		=	NULL;
CPlayer			*CManager::m_pPlayer	=	NULL;
CManager::MODE	 CManager::m_mode		=	CManager::MODE_NONE;

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CManager::CManager()
{
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CManager::~CManager()
{
}

//*****************************************************************************
//�����_���[�̐���
//*****************************************************************************
void CManager::CreateRenderer(HWND hWnd)
{
	//�����_���[�̃������m��
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
	}

	//�����_���[�̏���������
	m_pRenderer->Init(hWnd, true);	//true : �E�B���h�E���[�h�Afalse : �t���X�N���[�����[�h
}

//*****************************************************************************
//�L�[�{�[�h�̐���
//*****************************************************************************
HRESULT CManager::CreateKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//�L�[�{�[�h�̃������m��
	if (m_pKeyboard == NULL)
	{
		m_pKeyboard = new CKeyboard;
	}

	//�L�[�{�[�h�̏���������
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
	{
		return -1;
	}

	return S_OK;
}

//*****************************************************************************
//�R���g���[���[�̐���
//*****************************************************************************
HRESULT CManager::CreateController(HINSTANCE hInstance, HWND hWnd)
{
	//�R���g���[���[�̃������m��
	if (m_pController == NULL)
	{
		m_pController = new CController;
	}

	//�R���g���[���[�̏���������
	if (FAILED(m_pController->Init(hInstance, hWnd)))
	{
		return -1;
	}

	return S_OK;
}

//*****************************************************************************
//�T�E���h�̐���
//*****************************************************************************
void CManager::CreateSound(HWND hWnd)
{
	//�T�E���h�̃������m��
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
	}

	//�T�E���h�̏�����
	if (m_pSound != NULL)
	{
		m_pSound->Init(hWnd);
	}
}

//*****************************************************************************
//�c�@�̐���
//*****************************************************************************
void CManager::CreateLife(D3DXVECTOR3 pos)
{
	//�c�@�̃������m��
	if (m_pLife == NULL)
	{
		m_pLife = new CLife;
	}

	//�c�@�̏���������
	if (m_pLife != NULL)
	{
		m_pLife->Init(pos);
	}
}

//*****************************************************************************
//�^�C���̐���
//*****************************************************************************
void CManager::CreateTime(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, CNumber::TYPE type)
{
	//�^�C���̃������m��
	if (m_pTime == NULL)
	{
		m_pTime = new CTime;
	}

	//�^�C���̏���������
	if (m_pTime != NULL)
	{
		m_pTime->Init(pos, size, nTime, type);
	}
}

//*****************************************************************************
//�v���C���[�̐���
//*****************************************************************************
void CManager::CreatePlayer(void)
{
	//�v���C���[�̃������m��
	//if (m_pPlayer == NULL)
	//{
		m_pPlayer = new CPlayer;
	//}

	//�v���C���[�̏���������
	if (m_pPlayer != NULL)
	{
		m_pPlayer->Init();
	}
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd)
{
	//�L�[�{�[�h�̐���
	CreateKeyboard(hInstance, hWnd);

	//�R���g���[���[�̐���
	CreateController(hInstance, hWnd);

	//�����_���[�̐���
	CreateRenderer(hWnd);

	//�v���C���[�e�N�X�`���̃��[�h
	CPlayer::Load();

	//�G�l�~�[�e�N�X�`���̃��[�h
	CEnemy::Load();

	//�o���b�g�e�N�X�`���̃��[�h
	CBullet::Load();

	//�����e�N�X�`���̃��[�h
	CExplosion::Load();

	//�w�i�e�N�X�`���̃��[�h
	CBg::Load();

	//�A�C�e���e�N�X�`���̃��[�h
	CItem::Load();

	//�G�t�F�N�g�e�N�X�`���̃��[�h
	CEffect::Load();

	//�����e�N�X�`���̃��[�h
	CNumber::Load();

	//�o���A�e�N�X�`���̃��[�h
	CBarrier::Load();

	//UI�e�N�X�`���̃��[�h
	CUi::Load();

	//�t�F�[�h�e�N�X�`���̃��[�h
	CFade::Load();

	//�h�A�e�N�X�`���̃��[�h
	CDoor::Load();

	//�T�E���h�̐���
	CreateSound(hWnd);

	//�f�ʋ@BGM�̍Đ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM04);

	//���[�h�̐ݒ�
	SetMode(MODE_TITLE);

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CManager::Uninit(void)
{
	//�L�[�{�[�h�̏I������
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Uninit();
	}

	//�R���g���[���[�̏I������
	if (m_pController != NULL)
	{
		m_pController->Uninit();
	}

	//�S�Ẵ|���S���̏I������
	CScene::ReleaseAll();

	//�v���C���[�e�N�X�`���̃A�����[�h
	CPlayer::Unload();

	//�G�l�~�[�e�N�X�`���̃A�����[�h
	CEnemy::Unload();

	//�v���C���[�e�N�X�`���̃A�����[�h
	CBullet::Unload();

	//�����e�N�X�`���̃A�����[�h
	CExplosion::Unload();

	//�w�i�e�N�X�`���̃A�����[�h
	CBg::Unload();

	//�A�C�e���e�N�X�`���̃A�����[�h
	CItem::Unload();

	//�G�t�F�N�g�e�N�X�`���̃A�����[�h
	CEffect::Unload();

	//�����e�N�X�`���̃A�����[�h
	CNumber::Unload();

	//�o���A�e�N�X�`���̃A�����[�h
	CBarrier::Unload();

	//UI�e�N�X�`���̃A�����[�h
	CUi::Unload();

	//�t�F�[�h�e�N�X�`���̃A�����[�h
	CFade::Unload();

	//�h�A�e�N�X�`���̃A�����[�h
	CDoor::Unload();

	//���̎��̃��[�h�̏I������
	if (m_pMode != NULL)
	{
		m_pMode->Uninit();
	}

	//�`��̏I������
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
	}

	//�f�ʋ@BGM�̒�~
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM04);

	//�T�E���h�̏I������
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
	}
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CManager::Update(void)
{
	//�L�[�{�[�h�̍X�V����
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Update();
	}

	//�R���g���[���[�̍X�V����
	if (m_pController != NULL)
	{
		m_pController->Update();
	}

	//�X�V����
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	//���̎��̃��[�h�̍X�V����
	if (m_pMode != NULL)
	{
		m_pMode->Update();
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CManager::Draw(void)
{
	//�`�揈��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}

	//���̎��̃��[�h�̕`�揈��
	if (m_pMode != NULL)
	{
		m_pMode->Draw();
	}
}

//*****************************************************************************
//�����_���[�̎擾
//*****************************************************************************
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//*****************************************************************************
//�L�[�{�[�h�̎擾
//*****************************************************************************
CKeyboard *CManager::GetKeyboard(void)
{
	return m_pKeyboard;
}

//*****************************************************************************
//�R���g���[���[�̎擾
//*****************************************************************************
CController *CManager::GetController(void)
{
	return m_pController;
}

//*****************************************************************************
//�T�E���h�̎擾
//*****************************************************************************
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

//*****************************************************************************
//�c�@�̎擾
//*****************************************************************************
CLife *CManager::GetLife(void)
{
	return m_pLife;
}

//*****************************************************************************
//�^�C���̎擾
//*****************************************************************************
CTime *CManager::GetTime(void)
{
	return m_pTime;
}

//*****************************************************************************
//�v���C���[�̎擾
//*****************************************************************************
CPlayer *CManager::GetPlayer(void)
{
	return m_pPlayer;
}

//*****************************************************************************
//���[�h�̎擾
//*****************************************************************************
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//*****************************************************************************
//���[�h�̃|�C���^�̎擾
//*****************************************************************************
CMode *CManager::GetModePointer(void)
{
	return m_pMode;
}

//*****************************************************************************
//���[�h�̐؂�ւ�
//*****************************************************************************
void CManager::SetMode(MODE mode)
{
	if (m_pMode != NULL)
	{
		//���̎��̃��[�h�̏I������
		m_pMode->Uninit();
		delete m_pMode;
		m_pMode = NULL;
	}

	if (m_pMode == NULL)
	{
		m_mode = mode;

		switch (m_mode)
		{
		case MODE_TITLE:

			//�^�C�g���̐���
			m_pMode = new CTitle;

			//�^�C�g���̏���������
			m_pMode->Init();

			break;

		case MODE_SELECT:

			//�Z���N�g��ʂ̐���
			m_pMode = new CSelect;

			//�Z���N�g��ʂ̏���������
			m_pMode->Init();

			break;

		case MODE_GAME:

			//�Q�[���̐���
			m_pMode = new CGame;

			//�Q�[���̏���������
			m_pMode->Init();

			break;

		case MODE_GAME_CREAR:

			//�N���A��ʂ̐���
			m_pMode = new CGameClear;

			//�N���A��ʂ̏���������
			m_pMode->Init();

			break;

		case MODE_SCORE_ATTACK:

			//�X�R�A�A�^�b�N�̐���
			m_pMode = new CScoreAttack;

			//�X�R�A�A�^�b�N�̏���������
			m_pMode->Init();

			break;

		case MODE_SCORE_SCREEN:

			//�X�R�A�\����ʂ̐���
			m_pMode = new CScoreScreen;

			//�X�R�A�\����ʂ̏���������
			m_pMode->Init();

			break;

		case MODE_TUTORIAL:

			//�`���[�g���A���̐���
			m_pMode = new CTutorial;

			//�`���[�g���A���̏���������
			m_pMode->Init();

			break;

		case MODE_TUTORIAL_GAME:

			//�`���[�g���A���̐���
			m_pMode = new CTutorialGame;

			//�`���[�g���A���̏���������
			m_pMode->Init();

			break;

		case MODE_TUTORIAL_SCORE_ATTACK:

			//�`���[�g���A���̐���
			m_pMode = new CTutorialScoreAttack;

			//�`���[�g���A���̏���������
			m_pMode->Init();

			break;

		case MODE_RANKING:

			//�����L���O�̐���
			m_pMode = new CRanking;

			//�����L���O�̏���������
			m_pMode->Init();

			break;

		default:

			break;
		}
	}
}