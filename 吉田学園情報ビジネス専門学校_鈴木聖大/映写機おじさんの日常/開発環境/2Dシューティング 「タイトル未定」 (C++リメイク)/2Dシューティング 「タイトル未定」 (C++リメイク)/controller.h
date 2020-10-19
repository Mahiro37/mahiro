//=============================================================================
//
// �W���C�p�b�h�̏��� [controller.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

//*****************************************************************************
//�w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "input.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define BUTTON_UP		(0)	    //�\���L�[��
#define BUTTON_DOWN		(18000)	//�\���L�[��
#define BUTTON_LEFT		(27000)	//�\���L�[��
#define BUTTON_RIGHT	(9000)	//�\���L�[�E
#define CROSSZONE		(4500)	//�\���L�[�̋��e�͈�
#define	MAX_CONTROLLER	(4)		//�Ȃ��R���g���[���[�̍ő吔 

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CController :public CInput
{
public:
	//�{�^���̎�ނ̗񋓌^
	typedef enum
	{
		BUTTON_X = 0,	//X
		BUTTON_Y,		//Y
		BUTTON_A,		//X
		BUTTON_B,		//Y
		BUTTON_L1,		//LB
		BUTTON_R1,		//RB
		BUTTON_L2,		//BACK
		BUTTON_R2,		//START
		BUTTON_L3,		//L��������
		BUTTON_R3,		//R��������
		BUTTON_BACK,	//BACK
		BUTTON_START,	//START
		BUTTON_M,
		MAX_BUTTON
	}BUTTON;

	//�R���g���[���[�̓��͏��̍\����
	typedef struct
	{
		LPDIRECTINPUTDEVICE8 pDevice = NULL;	// �R���g���[���[�f�o�C�X
		DIJOYSTATE2 State;						//�R���g���[���[�̃v���X���
		DIJOYSTATE2 Trigger;					//�R���g���[���[�̃g���K�[���
		DIJOYSTATE2 Release;					//�R���g���[���[�̃v���X���
	}CONTROLLER;

	static HRESULT CALLBACK CController::GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/);
	CController();
	~CController();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Update(void);
	void ReleaseDInput(void);
	bool GetJoypadPress(int nButton);
	bool GetJoypadTrigger(int nButton);
	bool GetJoypadRelease(int nButton);
	DIJOYSTATE2 GetController(void);

private:
	static CONTROLLER m_Controller[MAX_CONTROLLER];
};

#endif