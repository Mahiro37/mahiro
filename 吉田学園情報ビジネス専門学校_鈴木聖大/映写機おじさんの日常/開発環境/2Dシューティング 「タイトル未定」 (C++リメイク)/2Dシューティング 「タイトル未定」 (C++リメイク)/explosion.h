//================================================================
//
// 爆発処理 [explosion.h]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//****************************************************************
//インクルードファイル
//****************************************************************
#include "scene2d.h"
#include "sound.h"

//****************************************************************
//クラス定義
//****************************************************************
class CExplosion :public CScene2D
{
public:
	CExplosion();
	~CExplosion();
	static CExplosion *Create(D3DXVECTOR3 pos, CSound::SOUND_LABEL SoundLabel);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetExplosion(D3DXVECTOR3 pos);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

#endif