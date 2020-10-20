//================================================================
//
// �L�[�{�[�h���� [keyboard.cpp]
// Author : Suzuki Mahiro
//
//================================================================

//****************************************************************
//�C���N���[�h�t�@�C��
//****************************************************************
#include "keyboard.h"
#include "input.h"

//****************************************************************
//�R���X�g���N�^
//****************************************************************
CKeyboard::CKeyboard()
{
	m_pDevice = NULL;	// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^

	for (int nCount = 0; nCount < NUM_KEY_MAX; nCount++)
	{
		m_aKeyState[nCount] = NULL;				// �L�[�{�[�h�̓��͏�񃏁[�N
		m_aKeyStateTrigger[nCount] = NULL;		//�L�[�{�[�h�̃g���K�[���
		m_aKeyStateRelease[nCount] = NULL;		//�L�[�{�[�h�̃����[�X���
	}
}

//****************************************************************
//�f�X�g���N�^
//****************************************************************
CKeyboard::~CKeyboard()
{
}

//****************************************************************
// �L�[�{�[�h�̏�����
//****************************************************************
HRESULT CKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//���͂̏�����
	CInput::Init(hInstance);

	// ���̓f�o�C�X(�L�[�{�[�h)�̍쐬
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�(�t�H�A�O���E���h����r�����[�h)
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}

//****************************************************************
// �L�[�{�[�h�̏I������
//****************************************************************
void CKeyboard::Uninit(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (m_pDevice != NULL)
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//****************************************************************
// �L�[�{�[�h�̍X�V����
//****************************************************************
void CKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��
	int nCntKey;						// ���[�J���ϐ�

	// �f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] =										//�u ^ �v�Fxor(�r���I�_���a)
				(m_aKeyState[nCntKey] & aKeyState[nCntKey])^aKeyState[nCntKey];	//�O�̒l(m_aKeyState) ^ ���̒l(aKeyState) & ���̒l(aKeyState)
																				//( 0 ^ 0 ) & 0 = 0  /  ( 0 ^ 1 ) & 1 = 1  /  ( 1 ^ 0 ) & 0 = 0  ( 1 ^ 1 ) & 1 = 0

			m_aKeyStateRelease[nCntKey] =
				(m_aKeyState[nCntKey] | aKeyState[nCntKey])&~aKeyState[nCntKey];//�O�̒l(m_aKeyState) ^ ���̒l(aKeyState) & ���̒l(aKeyState)

			m_aKeyState[nCntKey] = aKeyState[nCntKey];// �L�[�v���X����ۑ�
		}
	}
	// �L�[�{�[�h�̍X�V����
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		m_pDevice->Acquire();
	}
}

//****************************************************************
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//****************************************************************
bool CKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//****************************************************************
// �L�[�{�[�h�̃v���X��Ԃ��擾
//****************************************************************
bool CKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//****************************************************************
// �L�[�{�[�h�̃����[�X��Ԃ��擾
//****************************************************************
bool CKeyboard::GetKeyboardRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}