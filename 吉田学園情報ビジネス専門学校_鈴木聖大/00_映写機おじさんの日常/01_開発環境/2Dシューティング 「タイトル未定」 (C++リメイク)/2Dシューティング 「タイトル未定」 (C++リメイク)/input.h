//================================================================
//
// 入力処理 [input.h]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//****************************************************************
//インクルードファイル
//****************************************************************
#include "main.h"

//****************************************************************
//クラス定義
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
	static LPDIRECTINPUT8 m_pInput;	// DirectInputオブジェクトへのポインタ
};

#endif