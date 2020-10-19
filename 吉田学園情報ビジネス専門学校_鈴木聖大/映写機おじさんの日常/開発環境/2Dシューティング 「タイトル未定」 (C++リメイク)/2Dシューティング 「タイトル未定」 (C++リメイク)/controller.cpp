//=============================================================================
// 
// ジョイパッドの処理 [controller.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "controller.h"
#include "input.h"
#include "manager.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define DEADZONE		(2500)			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		(10000)			// 有効範囲の最大値
#define RANGE_MIN		(-10000)		// 有効範囲の最小値

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
CController::CONTROLLER CController::m_Controller[MAX_CONTROLLER] = {};

//*****************************************************************************
//ジョイスティック・デバイスを列挙（コールバック関数）
//*****************************************************************************
HRESULT CALLBACK CController::GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/)
{
	HRESULT hr;

	hr = m_pInput->CreateDevice(lpddi->guidInstance, &m_Controller[0].pDevice, NULL);
	if (FAILED(hr))
		return DIENUM_CONTINUE;	// 次のデバイスを列挙

	return DIENUM_CONTINUE;	// デバイスの列挙を中止
}

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CController::CController()
{
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CController::~CController()
{
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CController::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT	hr;

	//入力の初期化
	CInput::Init(hInstance);

	// コントローラーを探す
	m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);

	if (!m_Controller[0].pDevice)
		return false;

	for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
	{//ボタンの初期化
		m_Controller[0].Trigger.rgbButtons[nCntButton] = '\0';
	}

	// コントローラー用のデータ・フォーマットを設定
	hr = m_Controller[0].pDevice->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
		return false; // データフォーマットの設定に失敗

	// モードを設定（フォアグラウンド＆非排他モード）
	hr = m_Controller[0].pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
		return false; // モードの設定に失敗

	// 軸の値の範囲を設定
	// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
	// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
	// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドは、関係なし)
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = RANGE_MIN;
	diprg.lMax = RANGE_MAX;

	//左スティック
	// X軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_X;
	m_Controller[0].pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Y軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_Y;
	m_Controller[0].pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	//右スティック
	// Z軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_Z;
	m_Controller[0].pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Z回転の範囲を設定
	diprg.diph.dwObj = DIJOFS_RZ;
	m_Controller[0].pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// 各軸ごとに、無効のゾーン値を設定する。
	// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
	// 指定する値は、10000に対する相対値(2000なら20パーセント)。
	DIPROPDWORD	dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwHow = DIPH_BYOFFSET;
	dipdw.dwData = DEADZONE;

	//左スティック
	//X軸の無効ゾーンを設定
	dipdw.diph.dwObj = DIJOFS_X;
	m_Controller[0].pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	//Y軸の無効ゾーンを設定
	dipdw.diph.dwObj = DIJOFS_Y;
	m_Controller[0].pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

	//右スティック
	//Z軸の無効ゾーンを設定
	dipdw.diph.dwObj = DIJOFS_Z;
	m_Controller[0].pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	//Z回転の無効ゾーンを設定
	dipdw.diph.dwObj = DIJOFS_RZ;
	m_Controller[0].pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

	//コントローラー入力制御開始
	m_Controller[0].pDevice->Acquire();

	return S_OK;
}

//*****************************************************************************
//更新処理
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

		//デバイスからデータを取得
		if (SUCCEEDED(hr = m_Controller[0].pDevice->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
		{
			//左スティックの情報
			//トリガー情報を保存
			//m_ControllerStateTrigger.rgbButtons[nCntButton] = (m_ControllerState.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];
			//プレス情報
			m_Controller[0].State.lY = ControllerState.lY;
			m_Controller[0].State.lX = ControllerState.lX;
			m_Controller[0].State.lZ = ControllerState.lZ;
			m_Controller[0].State.lRz = ControllerState.lRz;

			//十字キーの情報
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				//十字キープレス情報を保存
				m_Controller[0].State.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];

			}

			//ボタンの情報
			for (nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
			{
				//キートリガー情報を保存
				m_Controller[0].Trigger.rgbButtons[nCntButton] = 
					(m_Controller[0].State.rgbButtons[nCntButton] & ControllerState.rgbButtons[nCntButton]) ^ ControllerState.rgbButtons[nCntButton];

				//キーリリース情報を保存
				m_Controller[0].Release.rgbButtons[nCntButton] = 
					(m_Controller[0].State.rgbButtons[nCntButton] | ControllerState.rgbButtons[nCntButton]) ^ ControllerState.rgbButtons[nCntButton];

				//キープレス情報を保存
				m_Controller[0].State.rgbButtons[nCntButton] = 
					ControllerState.rgbButtons[nCntButton];
			}
		}
		else
		{
			//コントローラーのアクセス権を取得
			m_Controller[0].pDevice->Acquire();
		}
	}
}

//*****************************************************************************
//ボタンのプレス状態を取得
//*****************************************************************************
bool CController::GetJoypadPress(int nButton)
{
	return(m_Controller[0].State.rgbButtons[nButton] & 0x80) ? true : false;
}

//*****************************************************************************
//ボタンのトリガー状態を取得
//*****************************************************************************
bool CController::GetJoypadTrigger(int nButton)
{
	return (m_Controller[0].Trigger.rgbButtons[nButton] & 0x80) ? true : false;;
}

//*****************************************************************************
//ボタンのリリース状態を取得
//*****************************************************************************
bool CController::GetJoypadRelease(int nButton)
{
	return(m_Controller[0].Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//*****************************************************************************
//コントローラの状態を取得
//*****************************************************************************
DIJOYSTATE2 CController::GetController(void)
{
	return m_Controller[0].State;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CController::ReleaseDInput(void)
{
	for (int nCntDevise = 0; nCntDevise < MAX_CONTROLLER; nCntDevise++)
	{
		//入力デバイスの開放
		if (m_Controller[nCntDevise].pDevice != NULL)
		{
			//コントローラのアクセス権を開放
			m_Controller[nCntDevise].pDevice->Unacquire();
			m_Controller[nCntDevise].pDevice->Release();
			m_Controller[nCntDevise].pDevice = NULL;
		}
	}

	//DirectInputオブジェクトの解放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}