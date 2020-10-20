//=============================================================================
//
// バレット処理 [bullet.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "bullet.h"
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "sound.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include "score.h"
#include "enemy.h"
#include "effect.h"
#include <time.h>
#include "score_attack.h"
#include "tutorial_score_attack.h"

//*****************************************************************************
//静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CBullet::CBullet()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fGravity = 0.0f;
	m_BulletState = BULLET_STATE_NONE;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CBullet::~CBullet()
{
}

//*****************************************************************************
//ロード処理
//*****************************************************************************
HRESULT CBullet::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "Textures/bullet.png", &m_pTexture);

	return S_OK;
}

//*****************************************************************************
//アンロード処理
//*****************************************************************************
void CBullet::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//*****************************************************************************
//バレットの生成
//*****************************************************************************
CBullet *CBullet::Create(D3DXVECTOR3 pos, int nCntCharge)
{
	//バレットのメモリ確保
	CBullet *pBullet = new CBullet;

	//バレットの初期化処理
	pBullet->Init(pos, nCntCharge);

	//オブジェクトの種類の設定
	pBullet->SetObjType(OBJTYPE_BULLET);

	return pBullet;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CBullet::Init(D3DXVECTOR3 pos, int nCntCharge)
{
	//サイズの設定
	SetSize(nCntCharge);

	//位置の設定
	SetPos(pos);

	//シーン2Dの初期化処理
	CScene2D::Init();

	//テクスチャの設定
	BindTexture(m_pTexture);

	//バレットのチャージ状態の設定
	m_BulletState = BULLET_STATE_NONE;

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CBullet::Uninit(void)
{
	//シーン2Dの終了処理
	CScene2D::Uninit();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CBullet::Update(void)
{
	D3DXVECTOR3 pos = CScene2D::GetPos();

	//ポジションを移動
	pos += m_move;

	//ポジションの設定
	SetPos(pos);

	switch (m_BulletState)
	{
	case BULLET_STATE_NONE:	//通常の弾だったとき

		break;

	case BULLET_STATE_MAX:	//MAXチャージの弾だったとき

		//エフェクトの生成
		CEffect::Create(pos, m_size, 0.0f, CEffect::EFFECT_TYPE_CHARGE_SHOT, ANIMATION_UNREPEAT, CSound::SOUND_LABEL_SE_HIT1);

		break;

	default:

		break;
	}

	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		//オブジェクトの取得
		CScene *pScene = GetScene(nCntScene);

		CEnemy *pEnemy = dynamic_cast<CEnemy*>(pScene);

		if (pEnemy != NULL)
		{
			//オブジェクトタイプの取得
			OBJTYPE objType = pEnemy->GetObjType();

			srand((unsigned int)time(NULL));
			int ItemPosY = rand() % 2 + 1;

			if (objType == OBJTYPE_BOSS_BALL)
			{ //ボスボールに当たったとき
				D3DXVECTOR3 targetPos = pEnemy->GetPos();

				if (targetPos.x - (BOSS_BALL_SIZE.x / 2) < pos.x + (m_size.x / 2) &&
					targetPos.x + (BOSS_BALL_SIZE.x / 2) > pos.x - (m_size.x / 2) &&
					targetPos.y - (BOSS_BALL_SIZE.y / 2) < pos.y + (m_size.y / 2) &&
					targetPos.y + (BOSS_BALL_SIZE.y / 2) > pos.y - (m_size.y / 2))
				{
					if (m_BulletState != BULLET_STATE_NOT_SHOOT)
					{
						//SEの再生
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT2);

						switch (m_BulletState)
						{
						case BULLET_STATE_NONE:	//通常の弾だったとき

							//ボールのライフを減らす
							pEnemy->SetBallLife(1);

							break;

						case BULLET_STATE_MAX:	//MAXチャージの弾だったとき

							//ボールのライフを減らす
							pEnemy->SetBallLife(3);

							break;

						default:

							break;
						}

						//ボールのライフを取得
						int nBallLife = pEnemy->GetBallLife();

						if (nBallLife <= 0)
						{
							//ファーストボールの生成
							CEnemy::Create(D3DXVECTOR3(targetPos.x + BALL_DIVISION_POS.x, targetPos.y + BALL_DIVISION_POS.y, 0.0f), FIRST_BALL_SIZE, D3DXVECTOR3(3.0f, -1.0f, 0.0f), OBJTYPE_FIRST_BALL);
							CEnemy::Create(D3DXVECTOR3(targetPos.x - BALL_DIVISION_POS.x, targetPos.y + BALL_DIVISION_POS.y, 0.0f), FIRST_BALL_SIZE, D3DXVECTOR3(-3.0f, -1.0f, 0.0f), OBJTYPE_FIRST_BALL);

							switch (m_BulletState)
							{
							case BULLET_STATE_NONE:	//通常の弾だったとき

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//スコアを加算する
									CScoreAttack::AddScore(1);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//スコアを加算する
									CTutorialScoreAttack::AddScore(1);
								}

								break;

							case BULLET_STATE_MAX:	//MAXチャージの弾だったとき

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//スコアを加算する
									CScoreAttack::AddScore(2);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//スコアを加算する
									CTutorialScoreAttack::AddScore(2);
								}

								//アイテムの生成
								switch (ItemPosY)
								{
								case 1:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								case 2:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(-1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								default:

									break;
								}

								break;

							default:

								break;
							}

							//エネミーの終了処理
							pEnemy->Uninit();
						}
						else
						{
							//状態の設定
							pEnemy->SetBallState(CEnemy::STATE_DAMAGE);
						}

						//爆発の生成
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//バレットの終了処理
						Uninit();
						return;

						break;
					}
					else
					{
						//爆発の生成
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//バレットの終了処理
						Uninit();
						return;

						break;
					}
				}
			}
			else if (objType == OBJTYPE_FIRST_BALL)
			{	//ファーストボールに当たったとき
				D3DXVECTOR3 targetPos = pEnemy->GetPos();

				if (targetPos.x - (FIRST_BALL_SIZE.x / 2) < pos.x + (m_size.x / 2) &&
					targetPos.x + (FIRST_BALL_SIZE.x / 2) > pos.x - (m_size.x / 2) &&
					targetPos.y - (FIRST_BALL_SIZE.y / 2) < pos.y + (m_size.y / 2) &&
					targetPos.y + (FIRST_BALL_SIZE.y / 2) > pos.y - (m_size.y / 2))
				{
					if (m_BulletState != BULLET_STATE_NOT_SHOOT)
					{
						//SEの再生
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT2);

						//ボールのライフを減らす
						pEnemy->SetBallLife(1);

						//ボールのライフを取得
						int nBallLife = pEnemy->GetBallLife();

						if (nBallLife == 0 || m_BulletState == BULLET_STATE_MAX)
						{
							//セカンドボールの生成
							CEnemy::Create(D3DXVECTOR3(targetPos.x + BALL_DIVISION_POS.x, targetPos.y + BALL_DIVISION_POS.y, 0.0f), SECOND_BALL_SIZE, D3DXVECTOR3(3.0f, -1.0f, 0.0f), OBJTYPE_SECOND_BALL);
							CEnemy::Create(D3DXVECTOR3(targetPos.x - BALL_DIVISION_POS.x, targetPos.y + BALL_DIVISION_POS.y, 0.0f), SECOND_BALL_SIZE, D3DXVECTOR3(-3.0f, -1.0f, 0.0f), OBJTYPE_SECOND_BALL);

							switch (m_BulletState)
							{
							case BULLET_STATE_NONE:	//通常の弾だったとき

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//スコアを加算する
									CScoreAttack::AddScore(1);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//スコアを加算する
									CTutorialScoreAttack::AddScore(1);
								}

								break;

							case BULLET_STATE_MAX:	//MAXチャージの弾だったとき

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//スコアを加算する
									CScoreAttack::AddScore(2);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//スコアを加算する
									CTutorialScoreAttack::AddScore(2);
								}

								//アイテムの生成
								switch (ItemPosY)
								{
								case 1:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								case 2:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(-1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								default:

									break;
								}

								break;

							default:

								break;
							}

							//エネミーの終了処理
							pEnemy->Uninit();
						}
						else
						{
							//状態の設定
							pEnemy->SetBallState(CEnemy::STATE_DAMAGE);
						}

						//爆発の生成
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//バレットの終了処理
						Uninit();
						return;

						break;
					}
					else
					{
						//爆発の生成
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//バレットの終了処理
						Uninit();
						return;

						break;
					}
				}
			}
			else if (objType == OBJTYPE_SECOND_BALL)
			{	//セカンドボールに当たったとき
				D3DXVECTOR3 targetPos = pEnemy->GetPos();

				if (targetPos.x - (SECOND_BALL_SIZE.x / 2) < pos.x + (m_size.x / 2) &&
					targetPos.x + (SECOND_BALL_SIZE.x / 2) > pos.x - (m_size.x / 2) &&
					targetPos.y - (SECOND_BALL_SIZE.y / 2) < pos.y + (m_size.y / 2) &&
					targetPos.y + (SECOND_BALL_SIZE.y / 2) > pos.y - (m_size.y / 2))
				{
					if (m_BulletState != BULLET_STATE_NOT_SHOOT)
					{
						//SEの再生
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT2);

						//ボールのライフを減らす
						pEnemy->SetBallLife(1);

						//ボールのライフを取得
						int nBallLife = pEnemy->GetBallLife();

						if (nBallLife == 0 || m_BulletState == BULLET_STATE_MAX)
						{
							//サードボールの生成
							CEnemy::Create(D3DXVECTOR3(targetPos.x + BALL_DIVISION_POS.x, targetPos.y + BALL_DIVISION_POS.y, 0.0f), THIRD_BALL_SIZE, D3DXVECTOR3(3.0f, -1.0f, 0.0f), OBJTYPE_THIRD_BALL);
							CEnemy::Create(D3DXVECTOR3(targetPos.x - BALL_DIVISION_POS.x, targetPos.y + BALL_DIVISION_POS.y, 0.0f), THIRD_BALL_SIZE, D3DXVECTOR3(-3.0f, -1.0f, 0.0f), OBJTYPE_THIRD_BALL);

							switch (m_BulletState)
							{
							case BULLET_STATE_NONE:	//通常の弾だったとき

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//スコアを加算する
									CScoreAttack::AddScore(10);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//スコアを加算する
									CTutorialScoreAttack::AddScore(10);
								}

								break;

							case BULLET_STATE_MAX:	//MAXチャージの弾だったとき

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

								//アイテムの生成
								switch (ItemPosY)
								{
								case 1:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								case 2:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(-1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								default:

									break;
								}

								break;

							default:

								break;
							}

							//エネミーの終了処理
							pEnemy->Uninit();
						}
						else
						{
							//状態の設定
							pEnemy->SetBallState(CEnemy::STATE_DAMAGE);
						}

						//爆発の生成
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//バレットの終了処理
						Uninit();
						return;

						break;
					}
					else
					{
						//爆発の生成
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//バレットの終了処理
						Uninit();
						return;

						break;
					}
				}
			}
			else if (objType == OBJTYPE_THIRD_BALL)
			{ //サードボールに当たったとき
				D3DXVECTOR3 targetPos = pEnemy->GetPos();

				if (targetPos.x - (THIRD_BALL_SIZE.x / 2) < pos.x + (m_size.x / 2) &&
					targetPos.x + (THIRD_BALL_SIZE.x / 2) > pos.x - (m_size.x / 2) &&
					targetPos.y - (THIRD_BALL_SIZE.y / 2) < pos.y + (m_size.y / 2) &&
					targetPos.y + (THIRD_BALL_SIZE.y / 2) > pos.y - (m_size.y / 2))
				{
					if (m_BulletState != BULLET_STATE_NOT_SHOOT)
					{
						//SEの再生
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT2);

						//ボールのライフを減らす
						pEnemy->SetBallLife(1);

						//ボールのライフを取得
						int nBallLife = pEnemy->GetBallLife();

						if (nBallLife == 0 || m_BulletState == BULLET_STATE_MAX)
						{
							switch (m_BulletState)
							{
							case BULLET_STATE_NONE:	//通常の弾だったとき

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

								break;

							case BULLET_STATE_MAX:	//MAXチャージの弾だったとき

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//スコアを加算する
									CScoreAttack::AddScore(200);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//スコアを加算する
									CTutorialScoreAttack::AddScore(200);
								}

								//アイテムの生成
								switch (ItemPosY)
								{
								case 1:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								case 2:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(-1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								default:

									break;
								}

								break;

							default:

								break;
							}

							//エネミーの終了処理
							pEnemy->Uninit();
						}
						else
						{
							//状態の設定
							pEnemy->SetBallState(CEnemy::STATE_DAMAGE);
						}

						//爆発の生成
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//バレットの終了処理
						Uninit();
						return;

						break;
					}
					else
					{
						//爆発の生成
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//バレットの終了処理
						Uninit();
						return;

						break;
					}
				}
			}
		}
	}

	if (pos.y <= -(m_size.y / 2))
	{ //バレットが画面外に出たとき
		//バレットの終了処理
		Uninit();
		return;
	}

	//シーン2Dの更新処理
	CScene2D::Update();
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CBullet::Draw(void)
{
	//シーンの描画処理
	CScene2D::Draw();
}

//*****************************************************************************
//弾の移動設定
//*****************************************************************************
void CBullet::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//*****************************************************************************
//弾のサイズ更新
//*****************************************************************************
void CBullet::SetSize(int nCntCharge)
{
	int nChargeSpeed = BULLET_CHARGE_SPEED;

	if (CManager::GetPlayer() != NULL)
	{
		nChargeSpeed = CManager::GetPlayer()->GetChargeSpeed();
	}

	m_size.x = (BULLET_SIZE_X / 3) + BULLET_SIZE_X * ((float)nCntCharge / (float)nChargeSpeed);
	m_size.y = (BULLET_SIZE_Y / 3) + BULLET_SIZE_Y * ((float)nCntCharge / (float)nChargeSpeed);

	if (m_size.x <= (BULLET_SIZE_X / 3))
	{
		m_size.x = (BULLET_SIZE_X / 3);
		m_size.y = (BULLET_SIZE_Y / 3);
	}

	//サイズの設定
	CScene2D::SetSize(m_size);
}

//*****************************************************************************
//バレットタイプの設定
//*****************************************************************************
void CBullet::SetBulletType(BULLET_STATE BulletState)
{
	m_BulletState = BulletState;
}

//*****************************************************************************
//バレットサイズの取得
//*****************************************************************************
D3DXVECTOR3 CBullet::GetBulletSize(void)
{
	return m_size;
}

//*****************************************************************************
//バレットの状態の取得
//*****************************************************************************
CBullet::BULLET_STATE CBullet::GetBulletState(void)
{
	return m_BulletState;
}