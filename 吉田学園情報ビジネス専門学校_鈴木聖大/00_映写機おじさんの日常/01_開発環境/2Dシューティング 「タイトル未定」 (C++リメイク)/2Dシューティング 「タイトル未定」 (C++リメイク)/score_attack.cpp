//=============================================================================
//
// スコアアタック処理 [score_attack.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include <time.h>
#include "score_attack.h"
#include "manager.h"
#include "keyboard.h"
#include "time.h"
#include "enemy.h"
#include "bg.h"
#include "player.h"
#include "score.h"
#include "life.h"
#include "sound.h"
#include "scene.h"
#include "ui.h"
#include "fade.h"
#include "ranking.h"
#include "message.h"

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
CScore *CScoreAttack::m_pScore = NULL;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CScoreAttack::CScoreAttack()
{
	m_pMessage = NULL;
	m_pFade = NULL;
	m_nTime = TIME_START_NUM;
	m_nCntResult = 0;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CScoreAttack::~CScoreAttack()
{
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CScoreAttack::Init(void)
{
	//背景の生成
	CBg::Create();

	//ファーストボールの生成
	CEnemy::Create(D3DXVECTOR3(800.0f, 200.0f, 0.0f), FIRST_BALL_SIZE, D3DXVECTOR3(3.0f, -2.0f, 0.0f), CScene::OBJTYPE_FIRST_BALL);
	CEnemy::Create(D3DXVECTOR3(400.0f, 200.0f, 0.0f), FIRST_BALL_SIZE, D3DXVECTOR3(-3.0f, -2.0f, 0.0f), CScene::OBJTYPE_FIRST_BALL);

	//プレイヤーの生成
	CManager::CreatePlayer();

	//タイムの生成
	CManager::CreateTime(D3DXVECTOR3((SCREEN_WIDTH / 2) + 20.0f, 80.0f, 0.0f), D3DXVECTOR3(40.0f, 50.0f, 0.0f), m_nTime, CNumber::TYPE_2);
	CManager::GetTime()->SetTime(CManager::GetTime()->GetSaveTime());
	CManager::GetTime()->SetStopTime(false);

	//タイマーUIの生成
	CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 70.0f, 0.0f), D3DXVECTOR3(110.0f, 130.0f, 0.0f), CUi::UI_TYPE_TIMER);

	//残機の生成
	CManager::CreateLife(D3DXVECTOR3(350.0f, 80.0f, 0.0f));
	int nLife = CManager::GetPlayer()->GetPlayerLife();
	CManager::GetLife()->SetLife(nLife);

	//残機UIの生成
	CUi::Create(D3DXVECTOR3(260.0f, 80.0f, 0.0f), D3DXVECTOR3(130.0f, 60.0f, 0.0f), CUi::UI_TYPE_ZANKI);

	//スコアの生成
	if (m_pScore == NULL)
	{
		m_pScore = CScore::Create(D3DXVECTOR3(1070.0f, 80.0f, 0.0f), D3DXVECTOR3(50.0f, 60.0f, 0.0f), 30.0f, 0, CNumber::TYPE_2);
	}
	if (m_pScore != NULL)
	{
		m_pScore->SetScore(m_pScore->GetScore());
	}

	//スコアUIの生成
	CUi::Create(D3DXVECTOR3(880.0f, 80.0f, 0.0f), D3DXVECTOR3(90.0f, 50.0f, 0.0f), CUi::UI_TYPE_SCORE);

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CScoreAttack::Uninit(void)
{
	//BGMの停止
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM05);

	//全てのメモリの開放処理
	CScene::ReleaseAllOtherThanFadeAndPlayer();

	if (m_pScore != NULL)
	{
		m_pScore->Uninit();
		m_pScore = NULL;
	}

	if (m_pMessage != NULL)
	{
		m_pMessage = NULL;
	}

	if (m_pFade != NULL)
	{
		m_pFade = NULL;
	}
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CScoreAttack::Update(void)
{
	int nTimeNow = 0;

	//時間の取得
	if (CManager::GetTime() != NULL)
	{
		nTimeNow = CManager::GetTime()->GetTime();
	}

	if (nTimeNow != 0)
	{
		bool bBall = false;

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			//オブジェクトの取得
			CScene *pScene = CScene::GetScene(nCntScene);

			//ダイナミックキャスト
			CEnemy *pEnemy = dynamic_cast<CEnemy*>(pScene);

			if (pEnemy != NULL)
			{
				bBall = true;
			}
		}

		if (bBall == false)
		{
			//ファーストボールの生成
			CEnemy::Create(D3DXVECTOR3(640.0f, -100.0f, 0.0f), FIRST_BALL_SIZE, D3DXVECTOR3(3.0f, -2.0f, 0.0f), CScene::OBJTYPE_FIRST_BALL);
			CEnemy::Create(D3DXVECTOR3(640.0f, -100.0f, 0.0f), FIRST_BALL_SIZE, D3DXVECTOR3(-3.0f, -2.0f, 0.0f), CScene::OBJTYPE_FIRST_BALL);
		}

		bool bPlayer = false;

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			//オブジェクトの取得
			CScene *pScene = CScene::GetScene(nCntScene);

			//ダイナミックキャスト
			CPlayer *pPlayer = dynamic_cast<CPlayer*>(pScene);

			if (pPlayer != NULL)
			{
				bPlayer = true;
			}
		}

		if (bPlayer == false)
		{
			if (CManager::GetTime() != NULL)
			{
				//時間を止める
				CManager::GetTime()->SetStopTime(true);

				//時間の保管
				CManager::GetTime()->SaveTime(nTimeNow);
			}

			if (CManager::GetPlayer()->GetPlayerLife() >= 0)
			{
				if (m_pFade == NULL)
				{
					//フェードの生成
					m_pFade = CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_SCORE_ATTACK);
				}
			}
		}
	}
	else
	{
		//SEの停止
		CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);

		if (m_pMessage == NULL)
		{
			//メッセージの生成
			m_pMessage = CMessage::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(400.0f, 200.0f, 0.0f), 60, CUi::UI_TYPE_FINISH, CManager::MODE_SCORE_SCREEN);
		}
	}
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CScoreAttack::Draw(void)
{
}

//*****************************************************************************
//スコアの加算処理
//*****************************************************************************
void CScoreAttack::AddScore(int nNum)
{
	m_pScore->AddScore(nNum);
}

//*****************************************************************************
//スコアの取得
//*****************************************************************************
CScore *CScoreAttack::GetScore(void)
{
	return m_pScore;
}