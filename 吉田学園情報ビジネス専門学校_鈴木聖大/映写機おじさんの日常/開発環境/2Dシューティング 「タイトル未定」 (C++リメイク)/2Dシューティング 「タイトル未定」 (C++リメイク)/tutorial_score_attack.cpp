//=============================================================================
//
// チュートリアル処理(スコアアタックモード) [tutorial_score_attack.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "tutorial_score_attack.h"
#include "manager.h"
#include "keyboard.h"
#include "controller.h"
#include "bg.h"
#include "enemy.h"
#include "bullet.h"
#include "fade.h"
#include "door.h"
#include "time.h"
#include "ui.h"
#include "score.h"
#include "player.h"
#include "scene.h"
#include "explosion.h"

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
CScore *CTutorialScoreAttack::m_pScore = NULL;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CTutorialScoreAttack::CTutorialScoreAttack()
{
	m_pDescriptionUi = NULL;
	m_bBall = false;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CTutorialScoreAttack::~CTutorialScoreAttack()
{
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CTutorialScoreAttack::Init(void)
{
	//BGMの再生
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM06);

	//チュートリアルモードの設定
	SetTutorialMode(TUTORIAL_MODE_SCORE_ATTACK);

	//背景の生成
	CBg::Create();

	//プレイヤーの生成
	CManager::CreatePlayer();

	//スコアアタックモードUIの生成
	CUi::Create(D3DXVECTOR3(320.0f, 105.0f, 0.0f), D3DXVECTOR3(350.0f, 150.0f, 0.0f), CUi::UI_TYPE_TUTORIAL_SCORE_ATTACK);

	//タイムの生成
	CManager::CreateTime(D3DXVECTOR3((SCREEN_WIDTH / 2) + 20.0f, 80.0f, 0.0f), D3DXVECTOR3(40.0f, 50.0f, 0.0f), TIME_START_NUM, CNumber::TYPE_2);
	CManager::GetTime()->SetStopTime(false);

	//タイマーUIの生成
	CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 70.0f, 0.0f), D3DXVECTOR3(110.0f, 130.0f, 0.0f), CUi::UI_TYPE_TIMER);

	//ドアの生成
	CDoor::Create(D3DXVECTOR3(229.8f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_SELECT);

	//スコアの生成
	if (m_pScore == NULL)
	{
		m_pScore = CScore::Create(D3DXVECTOR3(1070.0f, 80.0f, 0.0f), D3DXVECTOR3(50.0f, 60.0f, 0.0f), 30.0f, 0, CNumber::TYPE_2);
	}

	//スコアUIの生成
	CUi::Create(D3DXVECTOR3(880.0f, 80.0f, 0.0f), D3DXVECTOR3(90.0f, 50.0f, 0.0f), CUi::UI_TYPE_SCORE);

	//ボールの生成位置のUIの生成
	CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 210.0f, 0.0f), D3DXVECTOR3(120.0f, 120.0f, 0.0f), CUi::UI_TYPE_BALL_CREATE);

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CTutorialScoreAttack::Uninit(void)
{
	//BGMの停止
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM06);

	if (m_pScore != NULL)
	{
		m_pScore->SetScore(0);
		m_pScore->Uninit();
		m_pScore = NULL;
	}

	//全てのメモリの開放処理
	CScene::ReleaseAllOtherThanFadeAndPlayer();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CTutorialScoreAttack::Update(void)
{
	if (CScene::GetUpdateStop() == false)
	{
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			//オブジェクトの取得
			CScene *pScene = CScene::GetScene(nCntScene);

			//ダイナミックキャスト
			CEnemy *pEnemy = dynamic_cast<CEnemy*>(pScene);

			if (pEnemy != NULL)
			{
				m_bBall = true;

				break;
			}
			else
			{
				m_bBall = false;
			}
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

				break;
			}
		}

		CKeyboard *pKeyboard = CManager::GetKeyboard();
		int nTime = CManager::GetTime()->GetTime();

		if (pKeyboard->GetKeyboardTrigger(DIK_LEFT) || pKeyboard->GetKeyboardTrigger(DIK_RIGHT) || CManager::GetController()->GetJoypadTrigger(4) || CManager::GetController()->GetJoypadTrigger(5))
		{ //左矢印 または 右矢印 または L1 または R1を押したとき
			//チュートリアルモードの設定
			SetTutorialMode(TUTORIAL_MODE_GAME);

			//フェードの生成
			CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_TUTORIAL_GAME);
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_R) || CManager::GetController()->GetJoypadTrigger(1) || nTime == 0 || bPlayer == false)
		{ //Rキー または Yボタンを押したとき または タイムが0になったとき または プレイヤーが死んだとき
			//SEの停止
			CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);

			//チュートリアルモードの設定
			SetTutorialMode(TUTORIAL_MODE_SCORE_ATTACK);

			//フェードの生成
			CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_TUTORIAL_SCORE_ATTACK);
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_B) || CManager::GetController()->GetJoypadTrigger(6))
		{ //Bキー または L2を押したとき
			if (m_bBall == false)
			{
				CEnemy::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 210.0f, 0.0f), THIRD_BALL_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CScene::OBJTYPE_THIRD_BALL);
			}
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_N) || CManager::GetController()->GetJoypadTrigger(7))
		{ //Nキー または R2を押したとき
			if (m_bBall == false)
			{
				CEnemy::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 210.0f, 0.0f), SECOND_BALL_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CScene::OBJTYPE_SECOND_BALL);
			}
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_X) || CManager::GetController()->GetJoypadTrigger(0))
		{ //Xキー または Xボタンを押したとき
			if (m_pDescriptionUi == NULL)
			{
				m_pDescriptionUi = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CUi::UI_TYPE_DESCRIPTION);

				//SEの停止
				CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);
				CManager::GetSound()->Stop(CSound::SOUND_LABEL_DOOR_OPEN);

				//プレスカウントの初期化
				CManager::GetPlayer()->SetPressCount(0);

				for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
				{
					//オブジェクトの取得
					CScene *pScene = CScene::GetScene(nCntScene);

					//ダイナミックキャスト
					CBullet *pBullet = dynamic_cast<CBullet*>(pScene);

					if (pBullet != NULL)
					{
						//カラーの更新
						pBullet->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						//弾の移動の更新
						pBullet->SetMove(D3DXVECTOR3(0.0f, BULLET_SPEED_Y, 0.0f));

						//バレットポインタをNULLにする
						CManager::GetPlayer()->SetBulletPointerNULL();
					}
				}

				CScene::UpdateStop(true);
			}
		}
	}
	else
	{
		if (m_pDescriptionUi != NULL)
		{
			if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_X) || CManager::GetController()->GetJoypadTrigger(0))
			{ //Xキー または Xボタンを押したとき
				m_pDescriptionUi->Uninit();
				m_pDescriptionUi = NULL;

				//SEの再生
				CManager::GetSound()->Play(CSound::SOUND_LABEL_PAPER_CLOSE);

				CScene::UpdateStop(false);
			}
		}
	}
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CTutorialScoreAttack::Draw(void)
{
}

//*****************************************************************************
//スコアの加算
//*****************************************************************************
void CTutorialScoreAttack::AddScore(int nNum)
{
	if (m_pScore != NULL)
	{
		m_pScore->AddScore(nNum);
	}
}