//=============================================================================
//
// ドア処理 [door.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _DOOR_H_
#define _DOOR_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2d.h"
#include "manager.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define DOOR_SIZE	(D3DXVECTOR3(160.0f, 280.0f, 0.0f))	//ドアのサイズ

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CDoor :public CScene2D
{
public:
	CDoor();
	~CDoor();
	static HRESULT Load(void);
	static void Unload(void);
	static CDoor *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE mode);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_pos;
	CManager::MODE m_mode;
	int m_nCntLit;
};

#endif