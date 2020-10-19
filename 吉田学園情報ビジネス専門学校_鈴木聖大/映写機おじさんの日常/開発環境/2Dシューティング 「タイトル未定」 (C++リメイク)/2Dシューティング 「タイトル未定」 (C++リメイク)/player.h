//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2d.h"
#include "item.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define PLAYER_TEXTURE		(1)										//テクスチャの種類
#define PLAYER_LIFE			(3)										//プレイヤーの残機数
#define PLAYER_SIZE			(D3DXVECTOR3(120.0f,150.0f,0.0f))		//プレイヤーのサイズ
#define PLAYER_RADIUS		(45.0f)									//プレイヤーの半径

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CBarrier;
class CBullet;
class CEffect;
class CMessage;

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CPlayer :public CScene2D
{
public:
	//プレイヤーの状態
	typedef enum
	{
		PLAYER_STATE_NONE = 0,		//通常
		PLAYER_STATE_INVINCIBLE,	//無敵
		PLAYER_STATE_DEATH,			//死亡
		PLAYER_STATE_MAX
	}PLAYER_STATE;
	
	CPlayer();
	~CPlayer();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void PlayerReset(void);

	void SetItemStock(CItem::ITEMTYPE ItemType);
	void SetPlayerAbility(void);
	void DeleteBarrierAbility(void);
	void SetLife(int nLife);
	void SetPlayerState(PLAYER_STATE state);
	void SetPressCount(int num);
	void SetBulletPointerNULL(void);

	D3DXVECTOR3 GetPos(void);
	CBarrier *GetBarrier(void);
	CBullet *GetBullet(void);
	int GetPlayerLife(void);
	int GetChargeSpeed(void);
	list<CItem::ITEMTYPE> GetStockItem(void);
	PLAYER_STATE GetPlayerState(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[PLAYER_TEXTURE];
	D3DXVECTOR3 m_pos;
	static int m_nLife;
	int m_nItemScore;
	float m_fPlayerSpeed;
	int m_nChargeSpeed;

	static CBarrier *m_pBarrier;
	CBullet *m_pBullet;
	CEffect *m_pEffect;
	CMessage *m_pMessage;

	list<CItem::ITEMTYPE> m_StockItem;

	PLAYER_STATE m_PlayerState;

	int m_nCntInvincible;
	int m_nInvincibleTime;
	int m_nCntPress;
	int m_nCntCharge;
	int m_nCntItem;
	static bool m_bRelease;
};

#endif