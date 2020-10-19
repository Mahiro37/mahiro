//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CUi;

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CPause :public CScene
{
public:
	//ポーズ画面の項目の列挙型
	typedef enum
	{
		MENU_CONTINUE = 0,		//続ける
		MENU_INSTRUCTION,		//操作説明
		MENU_START_OVER,		//最初からやり直す
		MENU_RETURN_TO_SELECT,	//セレクトに戻る
		MENU_MAX			
	}MENU;

	CPause();
	~CPause();
	static CPause *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CUi *m_apMenuUi[MENU_MAX];
	CUi *m_pUi;
	CUi *m_pInstructionUi;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	int m_nChoiceNum;
	bool m_bStickMove;
};

#endif