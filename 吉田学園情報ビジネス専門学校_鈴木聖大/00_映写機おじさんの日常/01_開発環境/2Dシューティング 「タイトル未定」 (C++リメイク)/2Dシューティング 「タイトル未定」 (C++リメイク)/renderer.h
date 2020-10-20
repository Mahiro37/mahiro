//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();							//コンストラクタ
	~CRenderer();							//デストラクタ
	HRESULT Init(HWND hWnd, bool bWindow);	//初期化処理
	void Uninit(void);						//終了処理
	void Update(void);						//更新処理
	void Draw();							//描画処理
	LPDIRECT3DDEVICE9 GetDevice(void);

private:
	void DrawFPS(int nCountFPS);

	LPDIRECT3D9 m_pD3D;
	LPD3DXFONT m_pFont;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
};

#endif