//=============================================================================
//
// スタート時のカウント処理 [start_count.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _START_COUNT_H_
#define _START_COUNT_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CNumber;
class CUi;

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CStartCount :public CScene
{
public:
	CStartCount();
	~CStartCount();
	static CStartCount *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CNumber *m_pNumber;
	CUi *m_pUi;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	int m_nStartNumber;
	int m_nCounter;
	int m_nCntStageNum;
};

#endif