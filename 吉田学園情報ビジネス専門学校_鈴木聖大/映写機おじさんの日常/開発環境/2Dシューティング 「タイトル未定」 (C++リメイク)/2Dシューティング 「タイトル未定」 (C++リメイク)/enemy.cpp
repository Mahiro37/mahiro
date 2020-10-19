//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "barrier.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define BALL_GRAVITY		(0.15f)		//ボールの重力
#define BALL_ELASTICITY		(11.0f)		//ボールの弾力
#define BOSS_BALL_LIFE		(15)		//ボスボールのライフ
#define FIRST_BALL_LIFE		(5)			//ファーストボールのライフ
#define SECOND_BALL_LIFE	(3)			//セカンドボールのライフ
#define THIRD_BALL_LIFE		(2)			//サードボールのライフ

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[TYPE_MAX] = {};

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CEnemy::CEnemy()
{
	m_EnemyState = STATE_NONE;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BarrierPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nBarrierLife = 0;
	m_nBallLife = 0;
	m_nDamageColorChange = 0;
	m_nInvincibleTime = 0;
	m_nCntCreateBall = 0;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CEnemy::~CEnemy()
{
}

//*****************************************************************************
//ロード処理
//*****************************************************************************
HRESULT CEnemy::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "Textures/ball/ball00.png", &m_pTexture[TYPE_FIRST]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ball/ball01.png", &m_pTexture[TYPE_SECOND]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ball/ball02.png", &m_pTexture[TYPE_THIRD]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ball/ball03.png", &m_pTexture[TYPE_BOSS]);

	return S_OK;
}

//*****************************************************************************
//アンロード処理
//*****************************************************************************
void CEnemy::Unload(void)
{
	//テクスチャの破棄
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//敵の生成
//*****************************************************************************
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, OBJTYPE type)
{
	//エネミーのメモリ確保
	CEnemy *pEnemy = new CEnemy;

	if (pEnemy != NULL)
	{
		//エネミーの初期化処理
		pEnemy->Init(pos, size, move, type);
	}

	return pEnemy;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, OBJTYPE type)
{
	//オブジェクトの種類の設定
	SetObjType(type);

	//エネミーの設定
	SetEnemy(pos, move);

	//サイズの設定
	SetSize(size);

	switch (type)
	{
	case OBJTYPE_FIRST_BALL:

		//テクスチャの割り当て
		BindTexture(m_pTexture[TYPE_FIRST]);

		m_nBallLife = FIRST_BALL_LIFE;

		break;

	case OBJTYPE_SECOND_BALL:

		//テクスチャの割り当て
		BindTexture(m_pTexture[TYPE_SECOND]);

		m_nBallLife = SECOND_BALL_LIFE;

		break;

	case OBJTYPE_THIRD_BALL:

		//テクスチャの割り当て
		BindTexture(m_pTexture[TYPE_THIRD]);

		m_nBallLife = THIRD_BALL_LIFE;

		break;

	case OBJTYPE_BOSS_BALL:

		//テクスチャの割り当て
		BindTexture(m_pTexture[TYPE_BOSS]);

		m_nBallLife = BOSS_BALL_LIFE;

		break;

	default:

		break;
	}

	//シーン2Dの初期化
	CScene2D::Init();

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CEnemy::Uninit(void)
{
	//シーン2Dの終了処理
	CScene2D::Uninit();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CEnemy::Update(void)
{
	//シーン2Dの更新処理
	CScene2D::Update();

	D3DXVECTOR3 pos = GetPos();

	//重力を加算
	m_move.y += BALL_GRAVITY;

	//移動処理
	pos += m_move;

	//ボールタイプの取得
	OBJTYPE BallType = GetObjType();

	switch (BallType)
	{
	case OBJTYPE_BOSS_BALL:

		//反射処理
		Reflection(pos, BOSS_BALL_SIZE);

		//回転処理
		RotationEnemy(pos, BOSS_BALL_SIZE, m_move);

		//雑魚を生成するカウントを進める
		m_nCntCreateBall++;

		if (m_nCntCreateBall == 480)
		{ //180フレーム経ったとき
			//サードボールの生成
			CEnemy::Create(D3DXVECTOR3(pos.x, pos.y - (BOSS_BALL_SIZE.y / 2) - (THIRD_BALL_SIZE.y / 2), 0.0f), THIRD_BALL_SIZE, D3DXVECTOR3(0.0f, -5.0f, 0.0f), OBJTYPE_THIRD_BALL);
		}
		else if (m_nCntCreateBall == 495)
		{ //195フレーム経ったとき
			//サードボールの生成
			CEnemy::Create(D3DXVECTOR3(pos.x, pos.y - (BOSS_BALL_SIZE.y / 2) - (THIRD_BALL_SIZE.y / 2), 0.0f), THIRD_BALL_SIZE, D3DXVECTOR3(0.0f, -5.0f, 0.0f), OBJTYPE_THIRD_BALL);
		}
		else if (m_nCntCreateBall == 510)
		{ //210フレーム経ったとき
			//カウントの初期化
			m_nCntCreateBall = 0;

		  //サードボールの生成
			CEnemy::Create(D3DXVECTOR3(pos.x, pos.y - (BOSS_BALL_SIZE.y / 2) - (THIRD_BALL_SIZE.y / 2), 0.0f), THIRD_BALL_SIZE, D3DXVECTOR3(0.0f, -5.0f, 0.0f), OBJTYPE_THIRD_BALL);
		}

		break;

	case OBJTYPE_FIRST_BALL:

		//反射処理
		Reflection(pos, FIRST_BALL_SIZE);

		//回転処理
		RotationEnemy(pos, FIRST_BALL_SIZE, m_move);

		break;

	case OBJTYPE_SECOND_BALL:

		//反射処理
		Reflection(pos,SECOND_BALL_SIZE);

		//回転処理
		RotationEnemy(pos,SECOND_BALL_SIZE, m_move);

		break;

	case OBJTYPE_THIRD_BALL:

		//反射処理
		Reflection(pos, THIRD_BALL_SIZE);

		//回転処理
		RotationEnemy(pos,  THIRD_BALL_SIZE, m_move);

		break;

	default:

		break;
	}

	//ポジションの設定
	SetPos(pos);

	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		//オブジェクトの取得
		CScene *pScene = GetScene(nCntScene);

		CBarrier *pBarrier = dynamic_cast<CBarrier*>(pScene);
		CBullet *pBullet = dynamic_cast<CBullet*>(pScene);

		if (pBarrier != NULL)
		{
			D3DXVECTOR3 BarrierPos = pBarrier->GetPos();

			switch (BallType)
			{
			case OBJTYPE_BOSS_BALL:

				if (sqrt(pow(pos.x - BarrierPos.x, 2) + pow(pos.y - BarrierPos.y, 2)) <= BOSS_BALL_RADIUS + BARRIER_RADIUS)
				{
					//SEの再生
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT1);

					//反射させる
					if (m_move.y > 0.0f)
					{
						if (m_move.x >= 0.0f)
						{
							if (pos.x >= BarrierPos.x)
							{
								m_move.y = m_move.y * -1.0f;
							}
							else
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
						}
						else
						{
							if (pos.x > BarrierPos.x)
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
							else
							{
								m_move.y = m_move.y * -1.0f;
							}
						}
					}
					else
					{
						m_move.x = m_move.x * -1.0f;
					}

					if (CManager::GetPlayer() != NULL)
					{
						if (CManager::GetPlayer()->GetPlayerState() == CPlayer::PLAYER_STATE_NONE)
						{
							//バリアUIの消去
							CManager::GetPlayer()->DeleteBarrierAbility();

							//プレイヤーの状態の設定
							CManager::GetPlayer()->SetPlayerState(CPlayer::PLAYER_STATE_INVINCIBLE);
						}
					}
				}

				break;

			case OBJTYPE_FIRST_BALL:

				if (sqrt(pow(pos.x - BarrierPos.x, 2) + pow(pos.y - BarrierPos.y, 2)) <= FIRST_BALL_RADIUS + BARRIER_RADIUS)
				{
					//SEの再生
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT1);

					//反射させる
					if (m_move.y > 0.0f)
					{
						if (m_move.x >= 0.0f)
						{
							if (pos.x >= BarrierPos.x)
							{
								m_move.y = m_move.y * -1.0f;
							}
							else
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
						}
						else
						{
							if (pos.x > BarrierPos.x)
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
							else
							{
								m_move.y = m_move.y * -1.0f;
							}
						}
					}
					else
					{
						m_move.x = m_move.x * -1.0f;
					}

					if (CManager::GetPlayer() != NULL)
					{
						if (CManager::GetPlayer()->GetPlayerState() == CPlayer::PLAYER_STATE_NONE)
						{
							//バリアUIの消去
							CManager::GetPlayer()->DeleteBarrierAbility();

							//プレイヤーの状態の設定
							CManager::GetPlayer()->SetPlayerState(CPlayer::PLAYER_STATE_INVINCIBLE);
						}
					}
				}

				break;

			case OBJTYPE_SECOND_BALL:

				if (sqrt(pow(pos.x - BarrierPos.x, 2) + pow(pos.y - BarrierPos.y, 2)) <= SECOND_BALL_RADIUS + BARRIER_RADIUS)
				{
					//SEの再生
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT1);

					//反射させる
					if (m_move.y > 0.0f)
					{
						if (m_move.x >= 0.0f)
						{
							if (pos.x >= BarrierPos.x)
							{
								m_move.y = m_move.y * -1.0f;
							}
							else
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
						}
						else
						{
							if (pos.x > BarrierPos.x)
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
							else
							{
								m_move.y = m_move.y * -1.0f;
							}
						}
					}
					else
					{
						m_move.x = m_move.x * -1.0f;
					}

					if (CManager::GetPlayer() != NULL)
					{
						if (CManager::GetPlayer()->GetPlayerState() == CPlayer::PLAYER_STATE_NONE)
						{
							//バリアUIの消去
							CManager::GetPlayer()->DeleteBarrierAbility();

							//プレイヤーの状態の設定
							CManager::GetPlayer()->SetPlayerState(CPlayer::PLAYER_STATE_INVINCIBLE);
						}
					}
				}

				break;

			case OBJTYPE_THIRD_BALL:

				if (sqrt(pow(pos.x - BarrierPos.x, 2) + pow(pos.y - BarrierPos.y, 2)) <= THIRD_BALL_RADIUS + BARRIER_RADIUS)
				{
					//SEの再生
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT1);

					//反射させる
					if (m_move.y > 0.0f)
					{
						if (m_move.x >= 0.0f)
						{
							if (pos.x > BarrierPos.x)
							{
								m_move.y = m_move.y * -1.0f;
							}
							else
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
						}
						else
						{
							if (pos.x > BarrierPos.x)
							{
								if (pos.y > BarrierPos.y)
								{
									m_move.y = m_move.y * -1.0f;
								}

								m_move.x = m_move.x * -1.0f;
							}
							else
							{
								m_move.y = m_move.y * -1.0f;
							}
						}
					}
					else
					{
						if (BarrierPos.x - m_BarrierPosOld.x < 0.0f)
						{
							if (m_move.x >= 0.0f)
							{
								m_move.x = m_move.x * -1.0f;
							}
						}
						else if (BarrierPos.x - m_BarrierPosOld.x >= 0.0f)
						{
							if (m_move.x < 0.0f)
							{
								m_move.x = m_move.x * -1.0f;
							}
						}
					}

					if (CManager::GetPlayer() != NULL)
					{
						if (CManager::GetPlayer()->GetPlayerState() == CPlayer::PLAYER_STATE_NONE)
						{
							//バリアUIの消去
							CManager::GetPlayer()->DeleteBarrierAbility();

							//プレイヤーの状態の設定
							CManager::GetPlayer()->SetPlayerState(CPlayer::PLAYER_STATE_INVINCIBLE);
						}
					}
				}

				break;

			default:

				break;
			}

			if (pBarrier != NULL)
			{
				//バリアのライフの取得
				m_nBarrierLife = pBarrier->GetBarrierLife();

				//バリアライフが０になったとき
				if (m_nBarrierLife == 0)
				{
					pBarrier = NULL;
				}
			}

			m_BarrierPosOld = BarrierPos;
		}
	}

	if (m_EnemyState == STATE_DAMAGE)
	{
		//カラーの更新
		SetColor(D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f));

		m_nDamageColorChange++;

		if (m_nDamageColorChange > 3)
		{
			m_nDamageColorChange = 0;

			m_EnemyState = STATE_NONE;

			if (m_EnemyState == STATE_NONE)
			{
				//カラーの更新
				SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CEnemy::Draw(void)
{
	//シーン2Dの描画処理
	CScene2D::Draw();
}

//*****************************************************************************
//設定処理
//*****************************************************************************
void CEnemy::SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	m_move = move;

	SetPos(pos);
}

//*****************************************************************************
//ボールの状態の設定
//*****************************************************************************
void CEnemy::SetBallState(ENEMY_STATE state)
{
	m_EnemyState = state;
}

//*****************************************************************************
//設定処理
//*****************************************************************************
void CEnemy::SetBallLife(int nDamage)
{
	m_nBallLife -= nDamage;
}

//*****************************************************************************
//反射処理
//*****************************************************************************
void CEnemy::Reflection(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//ボールタイプの取得
	OBJTYPE BallType = GetObjType();

	//左の壁に当たったとき
	if ((pos.x - (size.x / 2.0f)) < WALL_SIZE_X)
	{
		//壁の外に出たら戻す
		pos.x = WALL_SIZE_X + size.x;

		//反射させる
		if (m_move.x < 0.0f)
		{
			m_move.x = m_move.x * -1.0f;
		}

		switch (BallType)
		{
		case OBJTYPE_BOSS_BALL:

			//バウンド音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BOSS_BALL);

			break;

		case OBJTYPE_FIRST_BALL:

			//バウンド音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_FIRST_BALL);

			break;

		case OBJTYPE_SECOND_BALL:

			//バウンド音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SECOND_BALL);

			break;

		case OBJTYPE_THIRD_BALL:

			//バウンド音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_THIRD_BALL);

			break;

		default:

			break;
		}
	}

	//右の壁に当たったとき
	if ((pos.x + (size.x / 2.0f)) > SCREEN_WIDTH - WALL_SIZE_X)
	{
		//壁の外に出たら戻す
		pos.x = (SCREEN_WIDTH - WALL_SIZE_X) - (size.x / 2);

		//反射させる
		if (m_move.x > 0.0f)
		{
			m_move.x = m_move.x * -1.0f;
		}

		switch (BallType)
		{
		case OBJTYPE_BOSS_BALL:

			//バウンド音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BOSS_BALL);

			break;

		case OBJTYPE_FIRST_BALL:

			//バウンド音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_FIRST_BALL);

			break;

		case OBJTYPE_SECOND_BALL:

			//バウンド音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SECOND_BALL);

			break;

		case OBJTYPE_THIRD_BALL:

			//バウンド音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_THIRD_BALL);

			break;

		default:

			break;
		}
	}

	//床に当たったとき
	if ((pos.y + (size.y / 2.0f)) >= SCREEN_HEIGHT - FLOOR_SIZE_Y)
	{
		//反射させる
		m_move.y = BALL_ELASTICITY * -1.0f;

		switch (BallType)
		{
		case OBJTYPE_BOSS_BALL:

			//バウンド音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BOSS_BALL);

			break;

		case OBJTYPE_FIRST_BALL:

			//バウンド音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_FIRST_BALL);

			break;

		case OBJTYPE_SECOND_BALL:

			//バウンド音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SECOND_BALL);

			break;

		case OBJTYPE_THIRD_BALL:

			//バウンド音の再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_THIRD_BALL);

			break;

		default:

			break;
		}
	}
}

//*****************************************************************************
//回転処理
//*****************************************************************************
void CEnemy::RotationEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move)
{
	if (move.x > 0.0f)
	{ //右に移動しているとき
		//回転処理
		Rotation(pos, size, ROTATION_RIGHT);
	}
	else if (move.x < 0.0f)
	{ //左に移動しているとき
		//回転処理
		Rotation(pos, size, ROTATION_LEFT);
	}
}

//*****************************************************************************
//ボールのライフの取得
//*****************************************************************************
int CEnemy::GetBallLife(void)
{
	return m_nBallLife;
}