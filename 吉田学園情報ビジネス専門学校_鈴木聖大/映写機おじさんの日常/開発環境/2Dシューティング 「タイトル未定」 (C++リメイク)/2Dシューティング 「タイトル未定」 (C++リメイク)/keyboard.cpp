//================================================================
//
// キーボード処理 [keyboard.cpp]
// Author : Suzuki Mahiro
//
//================================================================

//****************************************************************
//インクルードファイル
//****************************************************************
#include "keyboard.h"
#include "input.h"

//****************************************************************
//コンストラクタ
//****************************************************************
CKeyboard::CKeyboard()
{
	m_pDevice = NULL;	// 入力デバイス(キーボード)へのポインタ

	for (int nCount = 0; nCount < NUM_KEY_MAX; nCount++)
	{
		m_aKeyState[nCount] = NULL;				// キーボードの入力情報ワーク
		m_aKeyStateTrigger[nCount] = NULL;		//キーボードのトリガー情報
		m_aKeyStateRelease[nCount] = NULL;		//キーボードのリリース情報
	}
}

//****************************************************************
//デストラクタ
//****************************************************************
CKeyboard::~CKeyboard()
{
}

//****************************************************************
// キーボードの初期化
//****************************************************************
HRESULT CKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//入力の初期化
	CInput::Init(hInstance);

	// 入力デバイス(キーボード)の作成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定(フォアグラウンド＆非排他モード)
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//****************************************************************
// キーボードの終了処理
//****************************************************************
void CKeyboard::Uninit(void)
{
	// 入力デバイス(キーボード)の開放
	if (m_pDevice != NULL)
	{
		// キーボードへのアクセス権を開放(入力制御終了)
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//****************************************************************
// キーボードの更新処理
//****************************************************************
void CKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報
	int nCntKey;						// ローカル変数

	// デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] =										//「 ^ 」：xor(排他的論理和)
				(m_aKeyState[nCntKey] & aKeyState[nCntKey])^aKeyState[nCntKey];	//前の値(m_aKeyState) ^ 今の値(aKeyState) & 今の値(aKeyState)
																				//( 0 ^ 0 ) & 0 = 0  /  ( 0 ^ 1 ) & 1 = 1  /  ( 1 ^ 0 ) & 0 = 0  ( 1 ^ 1 ) & 1 = 0

			m_aKeyStateRelease[nCntKey] =
				(m_aKeyState[nCntKey] | aKeyState[nCntKey])&~aKeyState[nCntKey];//前の値(m_aKeyState) ^ 今の値(aKeyState) & 今の値(aKeyState)

			m_aKeyState[nCntKey] = aKeyState[nCntKey];// キープレス情報を保存
		}
	}
	// キーボードの更新処理
	else
	{
		// キーボードへのアクセス権を取得
		m_pDevice->Acquire();
	}
}

//****************************************************************
// キーボードのトリガー状態を取得
//****************************************************************
bool CKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//****************************************************************
// キーボードのプレス状態を取得
//****************************************************************
bool CKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//****************************************************************
// キーボードのリリース状態を取得
//****************************************************************
bool CKeyboard::GetKeyboardRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}