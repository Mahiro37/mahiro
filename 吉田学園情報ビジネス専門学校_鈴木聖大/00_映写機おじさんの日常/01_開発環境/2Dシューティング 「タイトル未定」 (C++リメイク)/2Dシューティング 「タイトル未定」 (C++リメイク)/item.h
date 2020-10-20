//================================================================
//
// アイテム処理 [item.cpp]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//****************************************************************
//インクルードファイル
//****************************************************************
#include "scene2d.h"
#include <list>
using namespace std;

//****************************************************************
//マクロ定義
//****************************************************************
#define MAX_ITEM				(256)							//アイテムの最大数
#define MAX_ITEM_TEXTURE		(32)							//テクスチャの種類
#define ITEM_SIZE				(D3DXVECTOR3(50.0f,50.0f,0.0f))	//アイテムのサイズ
#define ITEM_RADIUS				(25.0f)							//アイテムの半径
#define ITEM_DIVISION_POS_X		(0.0f)							//分裂後の初期X座標
#define ITEM_DIVISION_POS_Y		(-20.0f)						//分裂後の初期Y座標

//****************************************************************
//クラス定義
//****************************************************************
class CItem :public CScene2D
{
public:
	//アイテムタイプの列挙体
	typedef enum
	{
		ITEMTYPE_NONE = 0,
		ITEMTYPE_PLAYER_SPEED_UP,	//スピードアップ
		ITEMTYPE_CHARGE_SPEED_UP,	//チャージスピードアップ
		ITEMTYPE_BARRIER,			//バリア
		ITEMTYPE_RESET,				//アイテムリセット
		ITEMTYPE_TIME_EXTENSION_1,	//時間延長(+1)
		ITEMTYPE_TIME_EXTENSION_2	//時間延長(+5)
	}ITEMTYPE;
	
	CItem();
	~CItem();
	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, OBJTYPE type);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move);

	ITEMTYPE GetItemType(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_ITEM_TEXTURE];
	static CItem *m_pItem[MAX_ITEM];
	LPDIRECT3DTEXTURE9 m_apScene[MAX_ITEM_TEXTURE];
	D3DXVECTOR3 m_move;
	int m_nItemCounter;
	ITEMTYPE m_ItemType;
};

#endif