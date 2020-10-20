//================================================================
//
// ���͏��� [input.h]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//****************************************************************
//�C���N���[�h�t�@�C��
//****************************************************************
#include "main.h"

//****************************************************************
//�N���X��`
//****************************************************************
class CInput
{
public:
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;	// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
};

#endif