//================================================================
//
// バレット処理 [bullet.h]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//****************************************************************
//インクルードファイル
//****************************************************************
#include "scene2d.h"

//****************************************************************
//マクロ定義
//****************************************************************
#define BULLET_SIZE_X			(35.0f)			//弾の最大Xサイズ
#define BULLET_SIZE_Y			(70.0f)			//弾の最大Yサイズ
#define BULLET_CHARGE_SPEED		(60)			//弾のチャージスピード
#define BULLET_SPEED_Y			(-10.0f)		//弾のスピード(Y軸)

//****************************************************************
//クラス定義
//****************************************************************
class CBullet :public CScene2D
{
public:
	//弾のチャージ状態の列挙型
	typedef enum
	{
		BULLET_STATE_NONE = 0,	//通常
		BULLET_STATE_MAX,		//MAXチャージ状態
		BULLET_STATE_NOT_SHOOT	//撃っていない
	}BULLET_STATE;

	CBullet();
	~CBullet();
	static CBullet *Create(D3DXVECTOR3 pos, int nCntCharge);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, int nCntCharge);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMove(D3DXVECTOR3 move);
	void SetSize(int nCntCharge);
	void SetBulletType(BULLET_STATE BulletState);
	
	D3DXVECTOR3 GetBulletSize(void);
	BULLET_STATE GetBulletState(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_size;
	float m_fGravity;
	BULLET_STATE m_BulletState;
};

#endif