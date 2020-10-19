//=============================================================================
//
// ���b�Z�[�W�\������ [message.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "message.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define COUNT_TIME		(60)	//�\�����鎞��

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
int CMessage::m_nCounter = 0;

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CMessage::CMessage()
{
	m_pUi = NULL;
	m_pFade = NULL;
	m_mode = CManager::MODE_NONE;
	m_nTime = 0;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CMessage::~CMessage()
{
}

//*****************************************************************************
//�X�^�[�g�J�E���g�̐���
//*****************************************************************************
CMessage *CMessage::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, CUi::UI_TYPE type, CManager::MODE mode)
{
	//���b�Z�[�W�̃������m��
	CMessage *pMessage = new CMessage;

	//���b�Z�[�W�̏���������
	if (pMessage != NULL)
	{
		pMessage->Init(pos, size, nTime, type, mode);
	}

	return pMessage;
}

HRESULT CMessage::Init(void)
{
	return S_OK;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CMessage::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, CUi::UI_TYPE type, CManager::MODE mode)
{
	switch (type)
	{
	case CUi::UI_TYPE_GAME_OVER:

		//SE�̍Đ�
		CManager::GetSound()->Play(CSound::SOUND_LABEL_GAME_OVER);

		break;

	default:

		break;
	}

	//UI�̐���
	if (m_pUi == NULL)
	{
		m_pUi = CUi::Create(pos, size, type);
	}

	if (m_pUi != NULL)
	{
		//���Ԃ̐ݒ�
		m_nTime = nTime;

		switch (CManager::GetMode())
		{
		case CManager::MODE_SCORE_ATTACK:

			//���[�h�̐ݒ�
			m_mode = CManager::MODE_SCORE_SCREEN;

			break;

		default:

			//���[�h�̐ݒ�
			m_mode = mode;

			break;
		}

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_START_OR_FINISH);

		//�V�[���̍X�V���~�߂�
		UpdateStop(true);
	}

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CMessage::Uninit(void)
{
	m_nCounter = 0;

	if (m_pUi != NULL)
	{
		m_pUi->Uninit();
		m_pUi = NULL;
	}

	if (m_pFade != NULL)
	{
		m_pFade = NULL;
	}

	//�������̊J������
	Release();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CMessage::Update(void)
{
	m_nCounter++;

	if (m_nCounter == m_nTime)
	{ //�w��̎��Ԍo�����Ƃ�
		//�t�F�[�h�̐���
		if (m_pFade == NULL)
		{
			m_pFade = CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), m_mode);
		}
	}

	if (m_pFade != NULL)
	{
		if (m_pFade->GetWrapBack() == true)
		{ //�t�F�[�h���܂�Ԃ����Ƃ�
			//�I������
			Uninit();
		}
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CMessage::Draw(void)
{
}