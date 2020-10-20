//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "keyboard.h"
#include "controller.h"
#include "bullet.h"
#include "sound.h"
#include "enemy.h"
#include "explosion.h"
#include "item.h"
#include "effect.h"
#include "score.h"
#include "life.h"
#include "barrier.h"
#include "fade.h"
#include "time.h"
#include "score_attack.h"
#include "tutorial_score_attack.h"
#include "game.h"
#include "time.h"
#include "message.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define PLAYER_MOVE						(5.0f)							//プレイヤーの移動量(初期値)
#define ANIMATION_SPEED					(6.0f)							//アニメーションの速さ
#define ANIMATION_PATTERN				(4.0f)							//アニメーションの行数
#define ANIMATION_LINE					(1.0f)							//アニメーションの列数
#define ITEM_SCORE						(1000)							//スコアアイテムのポイント数
#define PLAYER_SPEED_UP					(2.0f)							//自機のスピード上昇量
#define BULLET_CHARGE_SPEED_DECREASE	(-(BULLET_CHARGE_SPEED / 5))	//弾のチャージスピード減少量

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture[PLAYER_TEXTURE] = {};
int CPlayer::m_nLife = PLAYER_LIFE;
CBarrier *CPlayer::m_pBarrier = NULL;
bool CPlayer::m_bRelease = true;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CPlayer::CPlayer()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PlayerState = PLAYER_STATE_NONE;
	m_nItemScore = 0;
	m_fPlayerSpeed = 0.0f;
	m_nChargeSpeed = 0;
	m_pBullet = NULL;
	m_pEffect = NULL;
	m_pMessage = NULL;
	m_nCntInvincible = 0;
	m_nInvincibleTime = 0;
	m_nCntPress = 0;
	m_nCntCharge = 0;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CPlayer::~CPlayer()
{
}

//*****************************************************************************
//ロード処理
//*****************************************************************************
HRESULT CPlayer::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "Textures/player.png", &m_pTexture[0]);

	return S_OK;
}

//*****************************************************************************
//アンロード処理
//*****************************************************************************
void CPlayer::Unload(void)
{
	// テクスチャの破棄
	for (int nCount = 0; nCount < PLAYER_TEXTURE; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CPlayer::Init(void)
{
	//プレイヤーの状態設定
	m_PlayerState = PLAYER_STATE_NONE;

	if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK ||
		CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
	{
		//プレイヤーの移動速度(初期値)
		m_fPlayerSpeed = PLAYER_MOVE * 1.3f;

		//プレイヤーの弾のチャージ速度(初期値)
		m_nChargeSpeed = (int)(BULLET_CHARGE_SPEED * 0.6);
	}
	else
	{
		//プレイヤーの移動速度(初期値)
		m_fPlayerSpeed = PLAYER_MOVE;

		//プレイヤーの弾のチャージ速度(初期値)
		m_nChargeSpeed = BULLET_CHARGE_SPEED;
	}

	//ポジションの設定
	SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2), 600.0f, 0.0f));

	//サイズの設定
	SetSize(PLAYER_SIZE);

	//アニメーションの初期化
	InitAnimation(ANIMATION_SPEED, ANIMATION_PATTERN, ANIMATION_LINE);

	//シーン2Dの初期化
	CScene2D::Init();

	//テクスチャの割り当て
	BindTexture(m_pTexture[0]);

	//オブジェクトの種類の設定
	SetObjType(OBJTYPE_PLAYER);

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CPlayer::Uninit(void)
{
	//SEの停止
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);

	//シーン2Dの終了処理
	CScene2D::Uninit();

	if (m_pBullet != NULL)
	{
		m_pBullet = NULL;
	}

	if (m_pEffect != NULL)
	{
		m_pEffect = NULL;
	}

	if (m_pMessage != NULL)
	{
		m_pMessage = NULL;
	}
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CPlayer::Update(void)
{
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	DIJOYSTATE2 Controller = CManager::GetController()->GetController();

	//位置の取得
	m_pos = CScene2D::GetPos();

	//画面外に出さない処理
	if (m_pos.x - (PLAYER_SIZE.x / 2) <= WALL_SIZE_X)
	{
		m_pos.x = WALL_SIZE_X + (PLAYER_SIZE.x / 2);
	}
	if (m_pos.x + (PLAYER_SIZE.x / 2) >= SCREEN_WIDTH - WALL_SIZE_X)
	{
		m_pos.x = SCREEN_WIDTH - WALL_SIZE_X - (PLAYER_SIZE.x / 2);
	}

	//キーボードの入力処理
	if (pKeyboard->GetKeyboardPress(DIK_A) || Controller.lX < 0)
	{ //Aを押したとき または スティックを左に傾けたとき
		m_pos.x -= m_fPlayerSpeed;

		//アニメーションの更新処理
		CScene2D::UpdateAnimation(ANIMATION_REPEAT, NOT_INVERSION);
	}
	if (pKeyboard->GetKeyboardPress(DIK_D) || Controller.lX > 0)
	{ //Dを押したとき または スティックを右に傾けたとき
		m_pos.x += m_fPlayerSpeed;

		//アニメーションの更新処理
		CScene2D::UpdateAnimation(ANIMATION_REPEAT, INVERSION);
	}

	if (CManager::GetMode() == CManager::MODE_GAME ||
		CManager::GetMode() == CManager::MODE_SCORE_ATTACK ||
		CManager::GetMode() == CManager::MODE_TUTORIAL_GAME ||
		CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
	{
		if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) || CManager::GetController()->GetJoypadTrigger(2))
		{ //SPACEキー または Aボタンを押している間
			//SEの再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_CHARGE);
		}

		if (pKeyboard->GetKeyboardPress(DIK_SPACE) || CManager::GetController()->GetJoypadPress(2))
		{ //SPACEキー または Aボタンを押している間
			if (m_pBullet == NULL)
			{
				if (m_bRelease == true)
				{
					//弾の生成
					m_pBullet = CBullet::Create(D3DXVECTOR3(m_pos.x, m_pos.y - (PLAYER_SIZE.y / 2), m_pos.z), m_nCntPress);

					m_bRelease = false;
				}
			}
			if (m_pBullet != NULL)
			{
				//バレットタイプの設定
				m_pBullet->SetBulletType(CBullet::BULLET_STATE_NOT_SHOOT);

				//カラーの更新
				m_pBullet->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.7f));

				//プレス時間の加算
				m_nCntPress++;

				if (m_nCntPress >= m_nChargeSpeed)
				{ //マックスよりプレスしたとき
					//マックスの値を代入する
					m_nCntPress = m_nChargeSpeed;

					//マックス状態でプレスしている時間を加算
					m_nCntCharge++;

					if (m_nCntCharge <= 10)
					{ //15フレーム以下のとき
						//カラーの更新
						m_pBullet->SetColor(D3DXCOLOR(1.0f, 0.3f, 0.3f, 0.7f));
					}
					else if (m_nCntCharge > 10 && m_nCntCharge < 20)
					{ //15フレームよりプレスしたとき
						//カラーの更新
						m_pBullet->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.7f));
					}
					else if (m_nCntCharge == 20)
					{ //30フレームプレスしたとき
						m_nCntCharge = 0;
					}

					//SEの停止
					CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);
				}

				//弾の位置の更新
				m_pBullet->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y - (PLAYER_SIZE.y / 2), m_pos.z));

				//弾のサイズの更新
				m_pBullet->SetSize(m_nCntPress);
			}
		}
		if (pKeyboard->GetKeyboardRelease(DIK_SPACE) || CManager::GetController()->GetJoypadRelease(2))
		{ //SPACEキー または Aボタンを離したとき
			m_bRelease = true;

			//SEの停止
			CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);

			CBullet *pBullet = NULL;

			for (int nCntScene = MAX_OBJECT - 1; nCntScene >= 0; nCntScene--)
			{
				//オブジェクトの取得
				CScene *pScene = GetScene(nCntScene);

				//ダイナミックキャスト
				pBullet = dynamic_cast<CBullet*>(pScene);

				if (pBullet != NULL)
				{
					break;
				}
			}

			if (pBullet != NULL)
			{
				if (m_pBullet != NULL)
				{
					if (m_nCntPress == m_nChargeSpeed)
					{ //マックスチャージ時
						//バレットタイプの設定
						m_pBullet->SetBulletType(CBullet::BULLET_STATE_MAX);

						//サウンドの再生
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_MAX);
					}
					else
					{
						//バレットタイプの設定
						m_pBullet->SetBulletType(CBullet::BULLET_STATE_NONE);

						//サウンドの再生
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_NONE);
					}

					//カラーの更新
					m_pBullet->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

					//弾の移動の更新
					m_pBullet->SetMove(D3DXVECTOR3(0.0f, BULLET_SPEED_Y, 0.0f));
				}
			}

			if (m_pBullet != NULL)
			{
				//弾のポインタを初期化
				m_pBullet = NULL;
			}

			//弾のリリースカウントの初期化
			m_nCntPress = 0;
		}
	}

	if (m_pBarrier != NULL)
	{
		int nLife = m_pBarrier->GetBarrierLife();

		if (nLife == 0)
		{
			m_pBarrier = NULL;
		}
	}

	//プレイヤーが生きているとき
	if (m_PlayerState == PLAYER_STATE_NONE)
	{
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			//オブジェクトの取得
			CScene *pScene = GetScene(nCntScene);

			//ダイナミックキャスト
			CEnemy *pEnemy = dynamic_cast<CEnemy*>(pScene);
			CItem *pItem = dynamic_cast<CItem*>(pScene);

			if (pEnemy != NULL)
			{
				//オブジェクトタイプの取得
				OBJTYPE objType = pEnemy->GetObjType();

				if (objType == OBJTYPE_BOSS_BALL)
				{ //ファーストボールに当たったとき
					D3DXVECTOR3 targetPos = pEnemy->GetPos();

					if (sqrt(pow(m_pos.x - targetPos.x, 2) + pow((m_pos.y - 30.0f) - targetPos.y, 2)) <= PLAYER_RADIUS + BOSS_BALL_RADIUS)
					{
						m_PlayerState = PLAYER_STATE_DEATH;

						break;
					}
				}
				else if (objType == OBJTYPE_FIRST_BALL)
				{ //ファーストボールに当たったとき
					D3DXVECTOR3 targetPos = pEnemy->GetPos();

					if (sqrt(pow(m_pos.x - targetPos.x, 2) + pow((m_pos.y - 30.0f) - targetPos.y, 2)) <= PLAYER_RADIUS + FIRST_BALL_RADIUS)
					{
						m_PlayerState = PLAYER_STATE_DEATH;

						break;
					}
				}
				else if (objType == OBJTYPE_SECOND_BALL)
				{ //セカンドボールに当たったとき
					D3DXVECTOR3 targetPos = pEnemy->GetPos();

					if (sqrt(pow(m_pos.x - targetPos.x, 2) + pow((m_pos.y - 30.0f) - targetPos.y, 2)) <= PLAYER_RADIUS + SECOND_BALL_RADIUS)
					{
						m_PlayerState = PLAYER_STATE_DEATH;

						break;
					}
				}
				else if (objType == OBJTYPE_THIRD_BALL)
				{ //サードボールに当たったとき
					D3DXVECTOR3 targetPos = pEnemy->GetPos();

					if (sqrt(pow(m_pos.x - targetPos.x, 2) + pow((m_pos.y - 30.0f) - targetPos.y, 2)) <= PLAYER_RADIUS + THIRD_BALL_RADIUS)
					{
						m_PlayerState = PLAYER_STATE_DEATH;

						break;
					}
				}
			}

			if (pItem != NULL)
			{ //アイテムに当たったとき
				D3DXVECTOR3 targetPos = pItem->GetPos();

				if (sqrt(pow(m_pos.x - targetPos.x, 2) + pow((m_pos.y - 30.0f) - targetPos.y, 2)) <= PLAYER_RADIUS + ITEM_RADIUS)
				{
					//アイテムタイプの取得
					CItem::ITEMTYPE ItemType = pItem->CItem::GetItemType();

					//アイテムの終了処理
					CItem *pItem = (CItem*)pScene;
					pItem->Uninit();

					int nTime = 0;

					//当たったアイテムが
					switch (ItemType)
					{
						//自機の移動スピードアップだったとき
					case CItem::ITEMTYPE_PLAYER_SPEED_UP:

						//ストックアイテムの設定
						SetItemStock(CItem::ITEMTYPE_PLAYER_SPEED_UP);

						//エフェクトの生成
						CEffect::Create(D3DXVECTOR3(0.0f, -30.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 5.0f, CEffect::EFFECT_TYPE_GET_SPEED_UP, ANIMATION_UNREPEAT, CSound::SOUND_LABEL_SE_HIT1);

						break;

						//弾のチャージスピードアップだったとき
					case CItem::ITEMTYPE_CHARGE_SPEED_UP:

						//ストックアイテムの設定
						SetItemStock(CItem::ITEMTYPE_CHARGE_SPEED_UP);

						//エフェクトの生成
						CEffect::Create(D3DXVECTOR3(0.0f, -30.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 5.0f, CEffect::EFFECT_TYPE_GET_TWIN_BULLET, ANIMATION_UNREPEAT, CSound::SOUND_LABEL_SE_HIT1);

						break;

						//バリアだったとき
					case CItem::ITEMTYPE_BARRIER:

						//ストックアイテムの設定
						SetItemStock(CItem::ITEMTYPE_BARRIER);

						//エフェクトの生成
						CEffect::Create(D3DXVECTOR3(0.0f, -30.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 5.0f, CEffect::EFFECT_TYPE_GET_BARRIER, ANIMATION_UNREPEAT, CSound::SOUND_LABEL_SE_HIT1);

						break;

						//タイム加算(+1)だったとき
					case CItem::ITEMTYPE_TIME_EXTENSION_1:

						//残りタイムの加算
						nTime = CManager::GetTime()->GetTime();
						if (nTime != 0)
						{
							nTime += 1;
							CManager::GetTime()->SetTime(nTime);
						}

						if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
						{
							//スコアを加算する
							CScoreAttack::AddScore(20);
						}
						else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
						{
							//スコアを加算する
							CTutorialScoreAttack::AddScore(20);
						}

						//エフェクトの生成
						CEffect::Create(D3DXVECTOR3(0.0f, -30.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 5.0f, CEffect::EFFECT_TYPE_GET_SCORE, ANIMATION_UNREPEAT, CSound::SOUND_LABEL_SE_HIT1);

						break;

						//タイム加算(+5)だったとき
					case CItem::ITEMTYPE_TIME_EXTENSION_2:

						//残りタイムの加算
						nTime = CManager::GetTime()->GetTime();
						if (nTime != 0)
						{
							nTime += 5;
							CManager::GetTime()->SetTime(nTime);
						}

						if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
						{
							//スコアを加算する
							CScoreAttack::AddScore(100);
						}
						else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
						{
							//スコアを加算する
							CTutorialScoreAttack::AddScore(100);
						}

						//エフェクトの生成
						CEffect::Create(D3DXVECTOR3(0.0f, -30.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), 5.0f, CEffect::EFFECT_TYPE_GET_SCORE, ANIMATION_UNREPEAT, CSound::SOUND_LABEL_SE_HIT1);

						break;
					}

					break;
				}
			}
		}
	}

	//ポジションの設定
	SetPos(m_pos);

	//プレイヤーが死亡しているとき
	if (m_PlayerState == PLAYER_STATE_DEATH)
	{
		//爆発の生成
		CExplosion::Create(m_pos, CSound::SOUND_LABEL_SE_EXPLOSION);

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			//オブジェクトの取得
			CScene *pScene = CScene::GetScene(nCntScene);

			//ダイナミックキャスト
			CBullet *pBullet = dynamic_cast<CBullet*>(pScene);

			if (pBullet != NULL)
			{
				//爆発の生成
				D3DXVECTOR3 BulletPos = pBullet->GetPos();
				CExplosion::Create(BulletPos, CSound::SOUND_LABEL_SE_EXPLOSION);

				//弾の終了処理
				pBullet->Uninit();
			}
		}

		m_pBullet = NULL;
		m_bRelease = true;

		if (m_nLife > 0)
		{//残機が残っていたとき
			if (CManager::GetMode() == CManager::MODE_GAME ||
				CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
			{
				//残機を一つ減らす
				m_nLife--;
			}

			//プレイヤーの終了処理
			Uninit();
			return;
		}
		else if (m_nLife == 0)
		{//残機が残っていなかったとき
			//残機を一つ減らす
			m_nLife--;

			//メッセージの生成
			if (m_pMessage == NULL)
			{
				m_pMessage = CMessage::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(700.0f, 200.0f, 0.0f), 60, CUi::UI_TYPE_GAME_OVER, CManager::MODE_SELECT);
			}

			//プレイヤーのリセット
			PlayerReset();

			//プレイヤーの終了処理
			Uninit();
			return;
		}
	}

	//プレイヤーが無敵状態のとき
	if (m_PlayerState == PLAYER_STATE_INVINCIBLE)
	{
		CBarrier *pBarrier;

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			//オブジェクトの取得
			CScene *pScene = GetScene(nCntScene);

			//ダイナミックキャスト
			pBarrier = dynamic_cast<CBarrier*>(pScene);

			if (pBarrier != NULL)
			{
				break;
			}
		}

		m_nCntInvincible++;

		if (m_nCntInvincible == 10)
		{
			//カラーの更新
			if (pBarrier != NULL)
			{
				pBarrier->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}

			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
		if (m_nCntInvincible == 20)
		{
			m_nCntInvincible = 0;

			m_nInvincibleTime++;

			//カラーの更新
			if (pBarrier != NULL)
			{
				pBarrier->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		//指定の回数点滅したとき
		if (m_nInvincibleTime == 2)
		{
			m_nInvincibleTime = 0;

			//プレイヤーの状態設定
			m_PlayerState = PLAYER_STATE_NONE;
		}
	}


	//シーン2Dの更新
	CScene2D::Update();
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CPlayer::Draw(void)
{
	//シーン2Dの描画処理
	CScene2D::Draw();
}

//*****************************************************************************
//プレイヤーのリセット
//*****************************************************************************
void CPlayer::PlayerReset(void)
{
	//移動速度のリセット
	m_fPlayerSpeed = PLAYER_MOVE;

	//弾のチャージ速度のリセット
	m_nChargeSpeed = BULLET_CHARGE_SPEED;

	if (m_pBarrier != NULL)
	{
		if (CManager::GetPlayer()->GetPlayerState() == CPlayer::PLAYER_STATE_NONE)
		{
			//バリアのリセット
			m_pBarrier->SetBarrierLife(-m_pBarrier->GetBarrierLife());

			m_pBarrier->Uninit();

			m_pBarrier = NULL;
		}
	}
}

//*****************************************************************************
//アイテムのストック処理
//*****************************************************************************
void CPlayer:: SetItemStock(CItem::ITEMTYPE ItemType)
{
	//リストの最後尾に入れる
	m_StockItem.push_back(ItemType);

	if (m_StockItem.size() > 3)
	{ //リストの要素数が３を超えるとき
		//リストの先頭要素を消す (.erase(指定の要素) : 指定の要素を消す) (begin() : 先頭の要素)
		m_StockItem.erase(m_StockItem.begin());
	}

	//プレイヤーのアイテム効果のリセット
	PlayerReset();

	//プレイヤーのアイテム効果の更新
	SetPlayerAbility();

	//アイテムストックUIの更新処理
	CGame::UpdateStock(m_StockItem);
}

//*****************************************************************************
//プレイヤーのアイテム効果の更新処理
//*****************************************************************************
void CPlayer::SetPlayerAbility(void)
{
	for (const auto &count : m_StockItem)
	{
		switch (count)
		{
			//自機の移動スピードアップだったとき
		case CItem::ITEMTYPE_PLAYER_SPEED_UP:

			//移動スピードを上げる
			m_fPlayerSpeed += PLAYER_SPEED_UP;

			break;

			//弾のチャージスピードアップだったとき
		case CItem::ITEMTYPE_CHARGE_SPEED_UP:

			//弾のチャージスピードを減少させる
			m_nChargeSpeed += BULLET_CHARGE_SPEED_DECREASE;

			break;

			//バリアだったとき
		case CItem::ITEMTYPE_BARRIER:

			if (m_pBarrier == NULL)
			{
				//バリアの生成 
				m_pBarrier = CBarrier::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));
			}
			else if (m_pBarrier != NULL)
			{
				if (m_pBarrier->GetBarrierLife() == 0)
				{
					m_pBarrier = NULL;

					//バリアの生成 
					m_pBarrier = CBarrier::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));
				}
				else
				{
					//バリアのライフ設定
					m_pBarrier->SetBarrierLife(1);
				}
			}
		}
	}
}

//*****************************************************************************
//バリア効果を１つ消す処理
//*****************************************************************************
void CPlayer::DeleteBarrierAbility(void)
{
	//プレイヤーのアイテム効果のリセット
	PlayerReset();

	for (auto itr = m_StockItem.begin(); itr != m_StockItem.end(); itr++)
	{
		if (*itr == CItem::ITEMTYPE_BARRIER)
		{
			//指定のリストを消す
			m_StockItem.erase(itr);

			break;
		}
	}

	//プレイヤーのアイテム効果の更新
	SetPlayerAbility();

	//アイテムストックUIの更新処理
	CGame::UpdateStock(m_StockItem);
}

//*****************************************************************************
//プレイヤーのライフの設定
//*****************************************************************************
void CPlayer::SetLife(int nLife)
{
	m_nLife = nLife;
}

//*****************************************************************************
//プレイヤーの状態の設定
//*****************************************************************************
void CPlayer::SetPlayerState(PLAYER_STATE state)
{
	m_PlayerState = state;
}

//*****************************************************************************
//プレスカウントの設定
//*****************************************************************************
void CPlayer::SetPressCount(int num)
{
	m_nCntPress = num;

	m_bRelease = true;
}

//*****************************************************************************
//バレットポインタをNULLにする
//*****************************************************************************
void CPlayer::SetBulletPointerNULL(void)
{
	m_pBullet = NULL;
}

//*****************************************************************************
//ポジションの取得
//*****************************************************************************
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}

//*****************************************************************************
//バリアの取得
//*****************************************************************************
CBarrier *CPlayer::GetBarrier(void)
{
	return m_pBarrier;
}

//*****************************************************************************
//バレットの取得
//*****************************************************************************
CBullet *CPlayer::GetBullet(void)
{
	return m_pBullet;
}

//*****************************************************************************
//プレイヤーの残機の取得
//*****************************************************************************
int CPlayer::GetPlayerLife(void)
{
	return m_nLife;
}

//*****************************************************************************
//弾のチャージスピードの取得
//*****************************************************************************
int CPlayer::GetChargeSpeed(void)
{
	return m_nChargeSpeed;
}

//*****************************************************************************
//プレイヤーの残機の取得
//*****************************************************************************
list<CItem::ITEMTYPE> CPlayer::GetStockItem(void)
{
	return m_StockItem;
}

//*****************************************************************************
//プレイヤーの状態の取得
//*****************************************************************************
CPlayer::PLAYER_STATE CPlayer::GetPlayerState(void)
{
	return m_PlayerState;
}