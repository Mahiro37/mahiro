//================================================================
//
// バリア処理 [barrier.h]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _BARRIER_H_
#define _BARRIER_H_

//****************************************************************
//インクルードファイル
//****************************************************************
#include "scene2d.h"

//****************************************************************
//マクロ定義
//****************************************************************
#define BARRIER_SIZE	(D3DXVECTOR3(150.0f,150.0f,0.0f))	//バリアのサイズ
#define BARRIER_RADIUS	(75.0f)								//バリアの半径

//****************************************************************
//クラス定義
//****************************************************************
class CBarrier :public CScene2D
{
public:
	//バリアのタイプ
	typedef enum
	{
		BARRIER_TYPE_NONE = 0,
		BARRIER_TYPE_1,
		BARRIER_TYPE_2,
		BARRIER_TYPE_3,
		BARRIER_TYPE_SCORE,
		BARRIER_MAX
	}BARRIER_TYPE;

	CBarrier();
	~CBarrier();
	static CBarrier *Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetBarrierLife(int nBarrierLife);

	int GetBarrierLife(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	static int m_nBarrierLife;
};

#endif