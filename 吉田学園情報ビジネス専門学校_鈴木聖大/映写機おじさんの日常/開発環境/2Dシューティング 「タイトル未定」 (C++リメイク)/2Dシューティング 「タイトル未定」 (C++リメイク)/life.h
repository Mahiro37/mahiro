//=============================================================================
//
// ライフ処理 [life.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "number.h"

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CLife :public CScene
{
public:
	CLife();
	~CLife();
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetLife(int nLife);

private:
	static CNumber *m_pNumber;
	static int m_nLife;
};

#endif