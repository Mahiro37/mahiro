//=============================================================================
//
// フェード処理 [fade.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2d.h"
#include "manager.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define FADE_MAX	(30)	//フェードアニメーションの枚数

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CStartCount;

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CFade :public CScene2D
{
public:
	CFade();
	~CFade();
	static HRESULT Load(void);
	static void Unload(void);
	static CFade *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE mode);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetStageNum(int nStageNum);

	bool GetWrapBack(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[FADE_MAX];
	static CFade *m_pFade;
	static CStartCount *m_pStartCount;
	static CManager::MODE m_mode;
	int m_nCntFade;
	int m_nCntTexture;
	bool m_bWrapBack;
	static int m_nStageNum;
};

#endif