//=============================================================================
//
// マネジメント処理 [manager.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "keyboard.h"
#include "controller.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2d.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "sound.h"
#include "bg.h"
#include "enemy.h"
#include "item.h"
#include "effect.h"
#include "score.h"
#include "life.h"
#include "barrier.h"
#include "time.h"
#include "title.h"
#include "game.h"
#include "score_attack.h"
#include "select.h"
#include "tutorial.h"
#include "tutorial_game.h"
#include "tutorial_score_attack.h"
#include "GameClear.h"
#include "ScoreScreen.h"
#include "ranking.h"
#include "ui.h"
#include "fade.h"
#include "door.h"

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
CRenderer		*CManager::m_pRenderer	=	NULL;
CKeyboard		*CManager::m_pKeyboard	=	NULL;
CController		*CManager::m_pController=	NULL;
CSound			*CManager::m_pSound		=	NULL;
CMode			*CManager::m_pMode		=	NULL;
CTime			*CManager::m_pTime		=	NULL;
CLife			*CManager::m_pLife		=	NULL;
CPlayer			*CManager::m_pPlayer	=	NULL;
CManager::MODE	 CManager::m_mode		=	CManager::MODE_NONE;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CManager::CManager()
{
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CManager::~CManager()
{
}

//*****************************************************************************
//レンダラーの生成
//*****************************************************************************
void CManager::CreateRenderer(HWND hWnd)
{
	//レンダラーのメモリ確保
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
	}

	//レンダラーの初期化処理
	m_pRenderer->Init(hWnd, true);	//true : ウィンドウモード、false : フルスクリーンモード
}

//*****************************************************************************
//キーボードの生成
//*****************************************************************************
HRESULT CManager::CreateKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//キーボードのメモリ確保
	if (m_pKeyboard == NULL)
	{
		m_pKeyboard = new CKeyboard;
	}

	//キーボードの初期化処理
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
	{
		return -1;
	}

	return S_OK;
}

//*****************************************************************************
//コントローラーの生成
//*****************************************************************************
HRESULT CManager::CreateController(HINSTANCE hInstance, HWND hWnd)
{
	//コントローラーのメモリ確保
	if (m_pController == NULL)
	{
		m_pController = new CController;
	}

	//コントローラーの初期化処理
	if (FAILED(m_pController->Init(hInstance, hWnd)))
	{
		return -1;
	}

	return S_OK;
}

//*****************************************************************************
//サウンドの生成
//*****************************************************************************
void CManager::CreateSound(HWND hWnd)
{
	//サウンドのメモリ確保
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
	}

	//サウンドの初期化
	if (m_pSound != NULL)
	{
		m_pSound->Init(hWnd);
	}
}

//*****************************************************************************
//残機の生成
//*****************************************************************************
void CManager::CreateLife(D3DXVECTOR3 pos)
{
	//残機のメモリ確保
	if (m_pLife == NULL)
	{
		m_pLife = new CLife;
	}

	//残機の初期化処理
	if (m_pLife != NULL)
	{
		m_pLife->Init(pos);
	}
}

//*****************************************************************************
//タイムの生成
//*****************************************************************************
void CManager::CreateTime(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, CNumber::TYPE type)
{
	//タイムのメモリ確保
	if (m_pTime == NULL)
	{
		m_pTime = new CTime;
	}

	//タイムの初期化処理
	if (m_pTime != NULL)
	{
		m_pTime->Init(pos, size, nTime, type);
	}
}

//*****************************************************************************
//プレイヤーの生成
//*****************************************************************************
void CManager::CreatePlayer(void)
{
	//プレイヤーのメモリ確保
	//if (m_pPlayer == NULL)
	//{
		m_pPlayer = new CPlayer;
	//}

	//プレイヤーの初期化処理
	if (m_pPlayer != NULL)
	{
		m_pPlayer->Init();
	}
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd)
{
	//キーボードの生成
	CreateKeyboard(hInstance, hWnd);

	//コントローラーの生成
	CreateController(hInstance, hWnd);

	//レンダラーの生成
	CreateRenderer(hWnd);

	//プレイヤーテクスチャのロード
	CPlayer::Load();

	//エネミーテクスチャのロード
	CEnemy::Load();

	//バレットテクスチャのロード
	CBullet::Load();

	//爆発テクスチャのロード
	CExplosion::Load();

	//背景テクスチャのロード
	CBg::Load();

	//アイテムテクスチャのロード
	CItem::Load();

	//エフェクトテクスチャのロード
	CEffect::Load();

	//数字テクスチャのロード
	CNumber::Load();

	//バリアテクスチャのロード
	CBarrier::Load();

	//UIテクスチャのロード
	CUi::Load();

	//フェードテクスチャのロード
	CFade::Load();

	//ドアテクスチャのロード
	CDoor::Load();

	//サウンドの生成
	CreateSound(hWnd);

	//映写機BGMの再生
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM04);

	//モードの設定
	SetMode(MODE_TITLE);

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CManager::Uninit(void)
{
	//キーボードの終了処理
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Uninit();
	}

	//コントローラーの終了処理
	if (m_pController != NULL)
	{
		m_pController->Uninit();
	}

	//全てのポリゴンの終了処理
	CScene::ReleaseAll();

	//プレイヤーテクスチャのアンロード
	CPlayer::Unload();

	//エネミーテクスチャのアンロード
	CEnemy::Unload();

	//プレイヤーテクスチャのアンロード
	CBullet::Unload();

	//爆発テクスチャのアンロード
	CExplosion::Unload();

	//背景テクスチャのアンロード
	CBg::Unload();

	//アイテムテクスチャのアンロード
	CItem::Unload();

	//エフェクトテクスチャのアンロード
	CEffect::Unload();

	//数字テクスチャのアンロード
	CNumber::Unload();

	//バリアテクスチャのアンロード
	CBarrier::Unload();

	//UIテクスチャのアンロード
	CUi::Unload();

	//フェードテクスチャのアンロード
	CFade::Unload();

	//ドアテクスチャのアンロード
	CDoor::Unload();

	//その時のモードの終了処理
	if (m_pMode != NULL)
	{
		m_pMode->Uninit();
	}

	//描画の終了処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
	}

	//映写機BGMの停止
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM04);

	//サウンドの終了処理
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
	}
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CManager::Update(void)
{
	//キーボードの更新処理
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Update();
	}

	//コントローラーの更新処理
	if (m_pController != NULL)
	{
		m_pController->Update();
	}

	//更新処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	//その時のモードの更新処理
	if (m_pMode != NULL)
	{
		m_pMode->Update();
	}
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CManager::Draw(void)
{
	//描画処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}

	//その時のモードの描画処理
	if (m_pMode != NULL)
	{
		m_pMode->Draw();
	}
}

//*****************************************************************************
//レンダラーの取得
//*****************************************************************************
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//*****************************************************************************
//キーボードの取得
//*****************************************************************************
CKeyboard *CManager::GetKeyboard(void)
{
	return m_pKeyboard;
}

//*****************************************************************************
//コントローラーの取得
//*****************************************************************************
CController *CManager::GetController(void)
{
	return m_pController;
}

//*****************************************************************************
//サウンドの取得
//*****************************************************************************
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

//*****************************************************************************
//残機の取得
//*****************************************************************************
CLife *CManager::GetLife(void)
{
	return m_pLife;
}

//*****************************************************************************
//タイムの取得
//*****************************************************************************
CTime *CManager::GetTime(void)
{
	return m_pTime;
}

//*****************************************************************************
//プレイヤーの取得
//*****************************************************************************
CPlayer *CManager::GetPlayer(void)
{
	return m_pPlayer;
}

//*****************************************************************************
//モードの取得
//*****************************************************************************
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//*****************************************************************************
//モードのポインタの取得
//*****************************************************************************
CMode *CManager::GetModePointer(void)
{
	return m_pMode;
}

//*****************************************************************************
//モードの切り替え
//*****************************************************************************
void CManager::SetMode(MODE mode)
{
	if (m_pMode != NULL)
	{
		//その時のモードの終了処理
		m_pMode->Uninit();
		delete m_pMode;
		m_pMode = NULL;
	}

	if (m_pMode == NULL)
	{
		m_mode = mode;

		switch (m_mode)
		{
		case MODE_TITLE:

			//タイトルの生成
			m_pMode = new CTitle;

			//タイトルの初期化処理
			m_pMode->Init();

			break;

		case MODE_SELECT:

			//セレクト画面の生成
			m_pMode = new CSelect;

			//セレクト画面の初期化処理
			m_pMode->Init();

			break;

		case MODE_GAME:

			//ゲームの生成
			m_pMode = new CGame;

			//ゲームの初期化処理
			m_pMode->Init();

			break;

		case MODE_GAME_CREAR:

			//クリア画面の生成
			m_pMode = new CGameClear;

			//クリア画面の初期化処理
			m_pMode->Init();

			break;

		case MODE_SCORE_ATTACK:

			//スコアアタックの生成
			m_pMode = new CScoreAttack;

			//スコアアタックの初期化処理
			m_pMode->Init();

			break;

		case MODE_SCORE_SCREEN:

			//スコア表示画面の生成
			m_pMode = new CScoreScreen;

			//スコア表示画面の初期化処理
			m_pMode->Init();

			break;

		case MODE_TUTORIAL:

			//チュートリアルの生成
			m_pMode = new CTutorial;

			//チュートリアルの初期化処理
			m_pMode->Init();

			break;

		case MODE_TUTORIAL_GAME:

			//チュートリアルの生成
			m_pMode = new CTutorialGame;

			//チュートリアルの初期化処理
			m_pMode->Init();

			break;

		case MODE_TUTORIAL_SCORE_ATTACK:

			//チュートリアルの生成
			m_pMode = new CTutorialScoreAttack;

			//チュートリアルの初期化処理
			m_pMode->Init();

			break;

		case MODE_RANKING:

			//ランキングの生成
			m_pMode = new CRanking;

			//ランキングの初期化処理
			m_pMode->Init();

			break;

		default:

			break;
		}
	}
}