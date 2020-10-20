//=============================================================================
// 
// �W���C�p�b�h�̏��� [controller.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "controller.h"
#include "input.h"
#include "manager.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define DEADZONE		(2500)			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		(10000)			// �L���͈͂̍ő�l
#define RANGE_MIN		(-10000)		// �L���͈͂̍ŏ��l

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
CController::CONTROLLER CController::m_Controller[MAX_CONTROLLER] = {};

//*****************************************************************************
//�W���C�X�e�B�b�N�E�f�o�C�X��񋓁i�R�[���o�b�N�֐��j
//*****************************************************************************
HRESULT CALLBACK CController::GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/)
{
	HRESULT hr;

	hr = m_pInput->CreateDevice(lpddi->guidInstance, &m_Controller[0].pDevice, NULL);
	if (FAILED(hr))
		return DIENUM_CONTINUE;	// ���̃f�o�C�X���

	return DIENUM_CONTINUE;	// �f�o�C�X�̗񋓂𒆎~
}

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CController::CController()
{
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CController::~CController()
{
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CController::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT	hr;

	//���͂̏�����
	CInput::Init(hInstance);

	// �R���g���[���[��T��
	m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);

	if (!m_Controller[0].pDevice)
		return false;

	for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
	{//�{�^���̏�����
		m_Controller[0].Trigger.rgbButtons[nCntButton] = '\0';
	}

	// �R���g���[���[�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	hr = m_Controller[0].pDevice->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
		return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_Controller[0].pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
		return false; // ���[�h�̐ݒ�Ɏ��s

	// ���̒l�͈̔͂�ݒ�
	// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
	// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
	// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�́A�֌W�Ȃ�)
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = RANGE_MIN;
	diprg.lMax = RANGE_MAX;

	//���X�e�B�b�N
	// X���͈̔͂�ݒ�
	diprg.diph.dwObj = DIJOFS_X;
	m_Controller[0].pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Y���͈̔͂�ݒ�
	diprg.diph.dwObj = DIJOFS_Y;
	m_Controller[0].pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	//�E�X�e�B�b�N
	// Z���͈̔͂�ݒ�
	diprg.diph.dwObj = DIJOFS_Z;
	m_Controller[0].pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Z��]�͈̔͂�ݒ�
	diprg.diph.dwObj = DIJOFS_RZ;
	m_Controller[0].pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
	// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
	// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
	DIPROPDWORD	dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwHow = DIPH_BYOFFSET;
	dipdw.dwData = DEADZONE;

	//���X�e�B�b�N
	//X���̖����]�[����ݒ�
	dipdw.diph.dwObj = DIJOFS_X;
	m_Controller[0].pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	//Y���̖����]�[����ݒ�
	dipdw.diph.dwObj = DIJOFS_Y;
	m_Controller[0].pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

	//�E�X�e�B�b�N
	//Z���̖����]�[����ݒ�
	dipdw.diph.dwObj = DIJOFS_Z;
	m_Controller[0].pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	//Z��]�̖����]�[����ݒ�
	dipdw.diph.dwObj = DIJOFS_RZ;
	m_Controller[0].pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

	//�R���g���[���[���͐���J�n
	m_Controller[0].pDevice->Acquire();

	return S_OK;
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CController::Update(void)
{
	DIJOYSTATE2 ControllerState;
	//BYTE ControllerState[MAX_BUTTON];
	HRESULT hr;
	int nCntButton;
	//for (int nCntController = 0; nCntController < MAX_CONTROLLER; nCntController++)
	//{

	//}
	if (m_Controller[0].pDevice != NULL)
	{
		hr = m_Controller[0].pDevice->Poll();

		hr = m_Controller[0].pDevice->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState);

		//�f�o�C�X����f�[�^���擾
		if (SUCCEEDED(hr = m_Controller[0].pDevice->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
		{
			//���X�e�B�b�N�̏��
			//�g���K�[����ۑ�
			//m_ControllerStateTrigger.rgbButtons[nCntButton] = (m_ControllerState.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];
			//�v���X���
			m_Controller[0].State.lY = ControllerState.lY;
			m_Controller[0].State.lX = ControllerState.lX;
			m_Controller[0].State.lZ = ControllerState.lZ;
			m_Controller[0].State.lRz = ControllerState.lRz;

			//�\���L�[�̏��
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				//�\���L�[�v���X����ۑ�
				m_Controller[0].State.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];

			}

			//�{�^���̏��
			for (nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
			{
				//�L�[�g���K�[����ۑ�
				m_Controller[0].Trigger.rgbButtons[nCntButton] = 
					(m_Controller[0].State.rgbButtons[nCntButton] & ControllerState.rgbButtons[nCntButton]) ^ ControllerState.rgbButtons[nCntButton];

				//�L�[�����[�X����ۑ�
				m_Controller[0].Release.rgbButtons[nCntButton] = 
					(m_Controller[0].State.rgbButtons[nCntButton] | ControllerState.rgbButtons[nCntButton]) ^ ControllerState.rgbButtons[nCntButton];

				//�L�[�v���X����ۑ�
				m_Controller[0].State.rgbButtons[nCntButton] = 
					ControllerState.rgbButtons[nCntButton];
			}
		}
		else
		{
			//�R���g���[���[�̃A�N�Z�X�����擾
			m_Controller[0].pDevice->Acquire();
		}
	}
}

//*****************************************************************************
//�{�^���̃v���X��Ԃ��擾
//*****************************************************************************
bool CController::GetJoypadPress(int nButton)
{
	return(m_Controller[0].State.rgbButtons[nButton] & 0x80) ? true : false;
}

//*****************************************************************************
//�{�^���̃g���K�[��Ԃ��擾
//*****************************************************************************
bool CController::GetJoypadTrigger(int nButton)
{
	return (m_Controller[0].Trigger.rgbButtons[nButton] & 0x80) ? true : false;;
}

//*****************************************************************************
//�{�^���̃����[�X��Ԃ��擾
//*****************************************************************************
bool CController::GetJoypadRelease(int nButton)
{
	return(m_Controller[0].Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//*****************************************************************************
//�R���g���[���̏�Ԃ��擾
//*****************************************************************************
DIJOYSTATE2 CController::GetController(void)
{
	return m_Controller[0].State;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CController::ReleaseDInput(void)
{
	for (int nCntDevise = 0; nCntDevise < MAX_CONTROLLER; nCntDevise++)
	{
		//���̓f�o�C�X�̊J��
		if (m_Controller[nCntDevise].pDevice != NULL)
		{
			//�R���g���[���̃A�N�Z�X�����J��
			m_Controller[nCntDevise].pDevice->Unacquire();
			m_Controller[nCntDevise].pDevice->Release();
			m_Controller[nCntDevise].pDevice = NULL;
		}
	}

	//DirectInput�I�u�W�F�N�g�̉��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}