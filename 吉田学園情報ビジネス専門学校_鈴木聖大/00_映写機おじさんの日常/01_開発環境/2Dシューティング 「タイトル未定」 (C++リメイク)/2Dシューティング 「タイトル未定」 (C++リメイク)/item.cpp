//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "explosion.h"
#include "bullet.h"
#include "player.h"
#include "tutorial.h"
#include "time.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define ITEM_GRAVITY	(0.18f)	//アイテムの重力
#define ITEM_ELASTICITY	(-8.0f)	//アイテムの弾力
#define ITEM_CHANGE		(2)		//アイテムを切り替える間隔

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
CItem *CItem::m_pItem[MAX_ITEM] = {};
LPDIRECT3DTEXTURE9 CItem::m_pTexture[MAX_ITEM_TEXTURE] = {};

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CItem::CItem()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	memset(m_apScene, 0, sizeof(m_apScene));
	m_nItemCounter = 0;
	m_ItemType = ITEMTYPE_NONE;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CItem::~CItem()
{
}

//*****************************************************************************
//ロード処理
//*****************************************************************************
HRESULT CItem::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "Textures/item/item00.png", &m_pTexture[ITEMTYPE_PLAYER_SPEED_UP]);
	D3DXCreateTextureFromFile(pDevice, "Textures/item/item01.png", &m_pTexture[ITEMTYPE_CHARGE_SPEED_UP]);
	D3DXCreateTextureFromFile(pDevice, "Textures/item/item02.png", &m_pTexture[ITEMTYPE_BARRIER]);
	D3DXCreateTextureFromFile(pDevice, "Textures/item/item03.png", &m_pTexture[ITEMTYPE_TIME_EXTENSION_1]);
	D3DXCreateTextureFromFile(pDevice, "Textures/item/item04.png", &m_pTexture[ITEMTYPE_TIME_EXTENSION_2]);

	return S_OK;
}

//*****************************************************************************
//アンロード処理
//*****************************************************************************
void CItem::Unload(void)
{
	//テクスチャの破棄
	for (int nCount = 0; nCount < MAX_ITEM_TEXTURE; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//エネミーの生成
//*****************************************************************************
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, OBJTYPE type)
{
	//エネミーのメモリ確保
	CItem *pItem = new CItem;

	//エネミーの初期化処理
	pItem->Init(pos, size, move);

	//オブジェクトの種類の設定
	pItem->SetObjType(type);

	return pItem;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move)
{
	//エネミーの設定
	SetItem(pos, move);

	if (CManager::GetMode() == CManager::MODE_GAME || CManager::GetMode() == CManager::MODE_TUTORIAL_GAME)
	{
		//アイテムタイプの設定
		m_ItemType = ITEMTYPE_PLAYER_SPEED_UP;

		//テクスチャの割り当て
		BindTexture(m_pTexture[ITEMTYPE_PLAYER_SPEED_UP]);
	}
	else if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK || CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
	{
		//サイズの設定
		size = D3DXVECTOR3(ITEM_SIZE.x, ITEM_SIZE.y * 1.2f, ITEM_SIZE.z);

		//アイテムタイプの設定
		m_ItemType = ITEMTYPE_TIME_EXTENSION_1;

		//テクスチャの割り当て
		BindTexture(m_pTexture[ITEMTYPE_TIME_EXTENSION_1]);
	}

	//サイズの設定
	SetSize(size);

	//シーン2Dの初期化
	CScene2D::Init();

	//アイテムカウンターの初期化
	m_nItemCounter = 0;

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CItem::Uninit(void)
{
	//シーン2Dの終了処理
	CScene2D::Uninit();

	//アイテムカウンターの初期化
	m_nItemCounter = 0;
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CItem::Update(void)
{
	//ポジションの取得
	D3DXVECTOR3 pos = CScene2D::GetPos();

	//左の壁に当たったとき
	if ((pos.x - (ITEM_SIZE.x / 2)) <= WALL_SIZE_X)
	{
		//壁の外に出たら戻す
		pos.x = WALL_SIZE_X + (ITEM_SIZE.x / 2);

		//反射させる
		m_move.x = m_move.x * -1.0f;

		//バウンド音の再生
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_THIRD_BALL);
	}

	//右の壁に当たったとき
	if ((pos.x + (ITEM_SIZE.x / 2)) >= SCREEN_WIDTH - WALL_SIZE_X)
	{
		//壁の外に出たら戻す
		pos.x = (SCREEN_WIDTH - WALL_SIZE_X) - (ITEM_SIZE.x / 2);

		//反射させる
		m_move.x = m_move.x * -1.0f;

		//バウンド音の再生
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_THIRD_BALL);
	}

	//重力を加算
	m_move.y += ITEM_GRAVITY;

	pos += m_move;

	//ポジションの設定
	SetPos(pos);

	//シーン2Dの更新処理
	CScene2D::Update();

	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		//オブジェクトの取得
		CScene *pScene = GetScene(nCntScene);

		//ダイナミックキャスト
		CBullet *pBullet = dynamic_cast<CBullet*>(pScene);	//指定したクラスじゃなかったらNULLになる

		if (pBullet != NULL)
		{
			D3DXVECTOR3 targetPos = pBullet->GetPos();
			D3DXVECTOR3 BulletSize = pBullet->GetBulletSize();

			//弾に当たったとき
			if (targetPos.x - (BulletSize.x / 2) < pos.x + (ITEM_SIZE.x / 2) &&
				targetPos.x + (BulletSize.x / 2) > pos.x - (ITEM_SIZE.x / 2) &&
				targetPos.y - (BulletSize.y / 2) < pos.y + (ITEM_SIZE.y / 2) &&
				targetPos.y + (BulletSize.y / 2) > pos.y - (ITEM_SIZE.y / 2))
			{
				if (pBullet->GetBulletState() != CBullet::BULLET_STATE_NOT_SHOOT)
				{
					//SEの再生
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BOSS_BALL);

					m_nItemCounter++;

					m_move.y = ITEM_ELASTICITY;

					//爆発の生成
					CExplosion::Create(pBullet->GetPos(), CSound::SOUND_LABEL_SE_HIT3);

					//バレットの終了処理
					pBullet->Uninit();

					if (CManager::GetMode() == CManager::MODE_GAME || CManager::GetMode() == CManager::MODE_TUTORIAL_GAME)
					{
						switch (m_nItemCounter)
						{
						case 0:

							m_ItemType = ITEMTYPE_PLAYER_SPEED_UP;

							//テクスチャの更新
							BindTexture(m_pTexture[ITEMTYPE_PLAYER_SPEED_UP]);

							break;

						case 1:

							m_ItemType = ITEMTYPE_CHARGE_SPEED_UP;

							//テクスチャの更新
							BindTexture(m_pTexture[ITEMTYPE_CHARGE_SPEED_UP]);

							break;

						case 2:

							m_ItemType = ITEMTYPE_BARRIER;

							//テクスチャの更新
							BindTexture(m_pTexture[ITEMTYPE_BARRIER]);

							break;

						case 3:

							m_nItemCounter = 0;

							m_ItemType = ITEMTYPE_PLAYER_SPEED_UP;

							//テクスチャの更新
							BindTexture(m_pTexture[ITEMTYPE_PLAYER_SPEED_UP]);

							break;

						default:

							break;
						}
					}
					else if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK || CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
					{
						if (m_nItemCounter < 3)
						{ //アイテムカウントが3より小さいとき
							m_ItemType = ITEMTYPE_TIME_EXTENSION_1;
						}
						else
						{ //アイテムカウントが3以上のとき
							m_ItemType = ITEMTYPE_TIME_EXTENSION_2;

							//テクスチャの更新
							BindTexture(m_pTexture[ITEMTYPE_TIME_EXTENSION_2]);
						}
					}

					break;
				}
			}
		}
	}

	
	//画面より上に出たとき
	if (pos.y <= -(ITEM_SIZE.y / 2))
	{
		//上昇量を半分にする
		m_move.y = (m_move.y / 2);
	}
	//画面より下に出たとき
	if (pos.y >= SCREEN_HEIGHT + (ITEM_SIZE.y / 2))
	{
		//アイテムの終了処理
		Uninit();
	}
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CItem::Draw(void)
{
	//シーン2Dの描画処理
	CScene2D::Draw();
}

//*****************************************************************************
//設定処理
//*****************************************************************************
void CItem::SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	m_move = move;

	SetPos(pos);
}

//*****************************************************************************
//アイテムタイプの取得	
//*****************************************************************************
CItem::ITEMTYPE CItem::GetItemType(void)
{
	return m_ItemType;
}