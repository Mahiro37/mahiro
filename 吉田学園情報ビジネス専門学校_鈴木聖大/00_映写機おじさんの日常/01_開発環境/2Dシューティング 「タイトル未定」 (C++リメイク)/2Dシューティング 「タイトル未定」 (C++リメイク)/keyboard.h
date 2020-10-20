//================================================================
//
// キーボード処理 [keyboard.h]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//****************************************************************
//インクルードファイル
//****************************************************************
#include "main.h"
#include "input.h"

//****************************************************************
// マクロ定義
//****************************************************************
#define NUM_KEY_MAX (256)			// キー最大数

//****************************************************************
//クラス定義
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
	LPDIRECTINPUTDEVICE8 m_pDevice;			// 入力デバイス(キーボード)へのポインタ
	BYTE m_aKeyState[NUM_KEY_MAX];			//キーボードの入力情報ワーク
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	//キーボードのトリガー情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	//キーボードのリリース情報
};

#endif