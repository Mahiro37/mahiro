//================================================================
//
// �L�[�{�[�h���� [keyboard.h]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//****************************************************************
//�C���N���[�h�t�@�C��
//****************************************************************
#include "main.h"
#include "input.h"

//****************************************************************
// �}�N����`
//****************************************************************
#define NUM_KEY_MAX (256)			// �L�[�ő吔

//****************************************************************
//�N���X��`
//****************************************************************
class CKeyboard :public CInput
{
public:
	CKeyboard();
	~CKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(int nKey);
	bool GetKeyboardRelease(int nKey);

private:
	LPDIRECTINPUTDEVICE8 m_pDevice;			// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
	BYTE m_aKeyState[NUM_KEY_MAX];			//�L�[�{�[�h�̓��͏�񃏁[�N
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	//�L�[�{�[�h�̃g���K�[���
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	//�L�[�{�[�h�̃����[�X���
};

#endif