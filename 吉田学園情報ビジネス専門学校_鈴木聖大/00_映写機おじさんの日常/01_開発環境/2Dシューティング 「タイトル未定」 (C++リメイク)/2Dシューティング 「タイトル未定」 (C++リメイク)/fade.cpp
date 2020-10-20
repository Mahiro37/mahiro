//=============================================================================
//
// フェード処理 [fade.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "fade.h"
#include "renderer.h"
#include "manager.h"
#include "score_attack.h"
#include "game.h"
#include "tutorial_score_attack.h"
#include "ScoreScreen.h"
#include "start_count.h"
#include "time.h"
#include "player.h"
#include "life.h"
#include "score.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CFade::m_apTexture[FADE_MAX] = {};
CFade *CFade::m_pFade = NULL;
CStartCount *CFade::m_pStartCount = NULL;
int CFade::m_nStageNum = 0;
CManager::MODE CFade::m_mode = CManager::MODE_NONE;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CFade::CFade()
{
	m_nCntFade = 0;
	m_nCntTexture = 0;
	m_bWrapBack = false;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CFade::~CFade()
{
}

//*****************************************************************************
//ロード処理
//*****************************************************************************
HRESULT CFade::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade00.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade01.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade02.png", &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade03.png", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade04.png", &m_apTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade05.png", &m_apTexture[5]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade06.png", &m_apTexture[6]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade07.png", &m_apTexture[7]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade08.png", &m_apTexture[8]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade09.png", &m_apTexture[9]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade10.png", &m_apTexture[10]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade11.png", &m_apTexture[11]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade12.png", &m_apTexture[12]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade13.png", &m_apTexture[13]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade14.png", &m_apTexture[14]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade15.png", &m_apTexture[15]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade16.png", &m_apTexture[16]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade17.png", &m_apTexture[17]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade18.png", &m_apTexture[18]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade19.png", &m_apTexture[19]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade20.png", &m_apTexture[20]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade21.png", &m_apTexture[21]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade22.png", &m_apTexture[22]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade23.png", &m_apTexture[23]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade24.png", &m_apTexture[24]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade25.png", &m_apTexture[25]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade26.png", &m_apTexture[26]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade27.png", &m_apTexture[27]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade28.png", &m_apTexture[28]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade29.png", &m_apTexture[29]);

	return S_OK;
}

//*****************************************************************************
//アンロード処理
//*****************************************************************************
void CFade::Unload(void)
{
	// テクスチャの破棄
	for (int nCount = 0; nCount < FADE_MAX; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//数字の生成
//*****************************************************************************
CFade *CFade::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE mode)
{
	//フェードのメモリ確保
	if (m_pFade == NULL)
	{
		m_pFade = new CFade;

		if (m_pFade != NULL)
		{
			//フェードの初期化処理
			m_pFade->Init(pos, size, mode);
		}
	}

	return m_pFade;
}

HRESULT CFade::Init(void)
{
	return S_OK;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CFade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE mode)
{
	//BGMの再生
	CManager::GetSound()->Play(CSound::SOUND_LABEL_CLOSE);

	//モードの設定
	m_mode = mode;

	//オブジェクトの種類の設定
	SetObjType(OBJTYPE_FADE);

	//テクスチャの設定
	BindTexture(m_apTexture[0]);

	//位置の設定
	SetPos(pos);

	//サイズの設定
	SetSize(size);

	//シーン2Dの初期化
	CScene2D::Init();

	//折り返し判定の初期化
	m_bWrapBack = false;

	//カウントの初期化
	m_nCntFade = 0;
	m_nCntTexture = 0;

	//シーンの更新を止める
	UpdateStop(true);

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CFade::Uninit(void)
{
	//シーン2Dの終了処理
	CScene2D::Uninit();

	if (m_pFade != NULL)
	{
		m_pFade = NULL;
	}

	if (m_pStartCount != NULL)
	{
		m_pStartCount = NULL;
	}
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CFade::Update(void)
{
	if (m_bWrapBack == false)
	{ //フェードが折り返していないとき
		//フレームカウントを進める
		m_nCntFade++;

		if (m_nCntFade == 2)
		{ //2フレーム経ったら
			//フレームカウントを初期化する
			m_nCntFade = 0;

			//テクスチャカウントを進める
			m_nCntTexture++;

			if (m_nCntTexture < FADE_MAX)
			{ //テクスチャカウントがFADE_MAXより小さいとき
				//テクスチャの設定
				BindTexture(m_apTexture[m_nCntTexture]);
			}
			else
			{ //テクスチャカウントがFADE_MAX以上のとき
				if (CManager::GetMode() == CManager::MODE_GAME ||
					CManager::GetMode() == CManager::MODE_SCORE_ATTACK ||
					CManager::GetMode() == CManager::MODE_TUTORIAL_GAME ||
					CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK ||
					CManager::GetMode() == CManager::MODE_SCORE_SCREEN ||
					CManager::GetMode() == CManager::MODE_GAME_CREAR)
				{
					if (m_mode == CManager::MODE_SELECT)
					{
						if (CManager::GetPlayer() != NULL)
						{ //ライフのリセット
							if (CManager::GetPlayer()->GetPlayerLife() < PLAYER_LIFE)
							{ //ライフが減っていたとき
								//ライフの初期化
								CManager::GetPlayer()->SetLife(PLAYER_LIFE);
							}
						}

						if (CManager::GetMode() == CManager::MODE_GAME)
						{
							if (CManager::GetModePointer() != NULL)
							{
								//ステージ番号の初期化
								CGame *pGame = dynamic_cast<CGame*>(CManager::GetModePointer());
								pGame->SetStageNum(0);
							}
						}
					}

					if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK ||
						CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK ||
						CManager::GetMode() == CManager::MODE_SCORE_SCREEN)
					{
						if (m_mode == CManager::MODE_SCORE_SCREEN)
						{ //スコアスクリーンに遷移するとき
							if (CScoreAttack::GetScore() != NULL && CManager::GetTime() != NULL)
							{
								//スコアの取得
								int nScore = CScoreAttack::GetScore()->GetScore();

								//総合タイムの取得
								int nTotalTime = CManager::GetTime()->GetTotalTime();
								CManager::GetTime()->ResetTotalTime();

								//スコアスクリーンにスコアを保存
								CScoreScreen::SaveScore(nScore);

								//スコアスクリーンに総合タイムを保存
								CScoreScreen::SaveTotalTime(nTotalTime);

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//スコアの初期化
									CScoreAttack::GetScore()->SetScore(0);
								}

								//スコアスクリーンに残機を保存
								CScoreScreen::SavePlayerLife(CManager::GetPlayer()->GetPlayerLife());
							}
						}

						if (m_mode == CManager::MODE_SELECT ||
							m_mode == CManager::MODE_TUTORIAL_SCORE_ATTACK)
						{
							if (CManager::GetTime() != NULL)
							{ //時間のリセット
								if (CManager::GetTime()->GetSaveTime() < TIME_START_NUM)
								{ //タイムが減っていたとき
								  //タイムの初期化
									CManager::GetTime()->SaveTime(TIME_START_NUM);
								}
							}

							if (CScoreAttack::GetScore() != NULL)
							{
								//スコアの初期化
								CScoreAttack::GetScore()->SetScore(0);
							}
						}
					}
				}

				//判定をtrueにする
				m_bWrapBack = true;

				//モードの設定
				GetManager()->SetMode(m_mode);

				//BGMの再生
				CManager::GetSound()->Play(CSound::SOUND_LABEL_OPEN);
			}
		}
	}
	else if (m_bWrapBack == true)
	{ //フェードが折り返しているとき
		//フレームカウントを進める
		m_nCntFade++;

		if (m_nCntFade == 2)
		{ //2フレーム経ったら
			//フレームカウントを初期化する
			m_nCntFade = 0;

			//テクスチャカウントを退く
			m_nCntTexture--;
			
			if (m_nCntTexture >= 0)
			{ //テクスチャカウントが0以上のとき
				//テクスチャの設定
				BindTexture(m_apTexture[m_nCntTexture]);
			}
			else
			{ //テクスチャカウントが0より小さいとき
				//カウントの初期化
				m_nCntTexture = 0;

				//判定の初期化
				m_bWrapBack = false;

				if (m_mode == CManager::MODE_GAME || m_mode == CManager::MODE_SCORE_ATTACK)
				{ //ステージモードまたはスコアアタックモードのとき
					if (m_pStartCount == NULL)
					{
						//スタートカウントの生成
						m_pStartCount = CStartCount::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(120.0f, 150.0f, 0.0f));
					}

					if (m_mode == CManager::MODE_GAME)
					{ //ステージモードのとき
						if (CManager::GetModePointer() != NULL)
						{
							//ステージ番号の初期化
							CGame *pGame = dynamic_cast<CGame*>(CManager::GetModePointer());
							pGame->SetStageNum(pGame->GetStageNum());
						}
					}
				}
				else
				{
					//シーンの更新を進める
					UpdateStop(false);
				}

				//終了処理
				Uninit();
				return;
			}
		}
	}

	//シーン2Dの更新処理
	CScene2D::Update();
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CFade::Draw(void)
{
	//シーン2Dの描画処理
	CScene2D::Draw();
}

//*****************************************************************************
//ステージ番号の設定
//*****************************************************************************
void CFade::SetStageNum(int nStageNum)
{
	m_nStageNum = nStageNum;
}

//*****************************************************************************
//フェードが折り返しているかどうか
//*****************************************************************************
bool CFade::GetWrapBack(void)
{
	return m_bWrapBack;
}