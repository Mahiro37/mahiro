//=============================================================================
//
// ジョイパッドの処理 [controller.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "input.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define BUTTON_UP		(0)	    //十字キー上
#define BUTTON_DOWN		(18000)	//十字キー下
#define BUTTON_LEFT		(27000)	//十字キー左
#define BUTTON_RIGHT	(9000)	//十字キー右
#define CROSSZONE		(4500)	//十字キーの許容範囲
#define	MAX_CONTROLLER	(4)		//つなぐコントローラーの最大数 

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CController :public CInput
{
public:
	//ボタンの種類の列挙型
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
		BUTTON_L3,		//L押し込み
		BUTTON_R3,		//R押し込み
		BUTTON_BACK,	//BACK
		BUTTON_START,	//START
		BUTTON_M,
		MAX_BUTTON
	}BUTTON;

	//コントローラーの入力情報の構造体
	typedef struct
	{
		LPDIRECTINPUTDEVICE8 pDevice = NULL;	// コントローラーデバイス
		DIJOYSTATE2 State;						//コントローラーのプレス情報
		DIJOYSTATE2 Trigger;					//コントローラーのトリガー情報
		DIJOYSTATE2 Release;					//コントローラーのプレス情報
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