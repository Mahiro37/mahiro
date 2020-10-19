//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "pause.h"
#include "ui.h"
#include "manager.h"
#include "game.h"
#include "sound.h"
#include "keyboard.h"
#include "controller.h"
#include "fade.h"
#include "player.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CPause::CPause()
{
	memset(m_apMenuUi, 0, sizeof(m_apMenuUi));
	m_pUi = NULL;
	m_pInstructionUi = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nChoiceNum = MENU_CONTINUE;
	m_bStickMove = false;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CPause::~CPause()
{
}

//*****************************************************************************
//ポーズの生成
//*****************************************************************************
CPause *CPause::Create(void)
{
	//ポーズのメモリ確保
	CPause *pPause = new CPause;

	//ポーズの初期化処理
	if (pPause != NULL)
	{
		pPause->Init();
	}

	return pPause;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CPause::Init(void)
{
	//SEの停止
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);

	//シーンの更新を止める
	CScene::UpdateStop(true);

	//オブジェクトの種類の設定
	SetObjType(OBJTYPE_PAUSE);

	//ポーズ項目の生成
	m_apMenuUi[MENU_CONTINUE] = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 200.0f, 0.0f), D3DXVECTOR3(200.0f, 100.0f, 0.0f), CUi::UI_TYPE_CONTINUE);
	m_apMenuUi[MENU_CONTINUE]->SetPauseChoice(true);
	m_apMenuUi[MENU_INSTRUCTION] = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 320.0f, 0.0f), D3DXVECTOR3(260.0f, 110.0f, 0.0f), CUi::UI_TYPE_INSTRUCTION);
	m_apMenuUi[MENU_START_OVER] = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 440.0f, 0.0f), D3DXVECTOR3(250.0f, 100.0f, 0.0f), CUi::UI_TYPE_START_OVER);
	m_apMenuUi[MENU_RETURN_TO_SELECT] = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 560.0f, 0.0f), D3DXVECTOR3(260.0f, 110.0f, 0.0f), CUi::UI_TYPE_RETURN_TO_SELECT);
	
	//ポーズ枠UIの生成
	m_pUi = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(500.0f, 580.0f, 0.0f), CUi::UI_TYPE_PAUSE);

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CPause::Uninit(void)
{
	for (int nCount = 0; nCount < MENU_MAX; nCount++)
	{
		if (m_apMenuUi[nCount] != NULL)
		{
			m_apMenuUi[nCount]->Uninit();
			m_apMenuUi[nCount] = NULL;
		}
	}

	if (m_pUi != NULL)
	{
		m_pUi->Uninit();
		m_pUi = NULL;
	}

	//メモリの開放
	Release();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CPause::Update(void)
{
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	DIJOYSTATE2 Controller = CManager::GetController()->GetController();

	if (m_pInstructionUi == NULL)
	{
		if (m_apMenuUi[m_nChoiceNum] != NULL)
		{
			if (pKeyboard->GetKeyboardTrigger(DIK_S) || pKeyboard->GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0)
			{ //Aを押したとき または 下を押したとき または スティックを下に傾けたとき
				if (m_bStickMove == false)
				{
					//選択している項目を解除
					m_apMenuUi[m_nChoiceNum]->SetPauseChoice(false);

					//一つ下の項目に移動
					m_nChoiceNum++;

					if (m_nChoiceNum == MENU_MAX)
					{ //一番下の項目で下に移動したとき
						//一番上の項目に移動する
						m_nChoiceNum = 0;
					}

					//移動した項目を点滅させる
					m_apMenuUi[m_nChoiceNum]->SetPauseChoice(true);
				}

				//スティックの移動フラグを立てる
				m_bStickMove = true;
			}
			else if (pKeyboard->GetKeyboardTrigger(DIK_W) || pKeyboard->GetKeyboardTrigger(DIK_UP) || Controller.lY < 0)
			{ //Wを押したとき または 上を押したとき または スティックを上に傾けたとき
				if (m_bStickMove == false)
				{
					//選択している項目を解除
					m_apMenuUi[m_nChoiceNum]->SetPauseChoice(false);

					//一つ上の項目に移動
					m_nChoiceNum--;

					if (m_nChoiceNum < 0)
					{ //一番上の項目で上に移動したとき
						//一番下の項目に移動する
						m_nChoiceNum = MENU_MAX - 1;
					}

					//移動した項目を点滅させる
					m_apMenuUi[m_nChoiceNum]->SetPauseChoice(true);
				}

				//スティックの移動フラグを立てる
				m_bStickMove = true;
			}
		}

		if (CManager::GetController()->GetController().lX == 0 &&
			CManager::GetController()->GetController().lY == 0)
		{
			//スティックの移動フラグを下げる
			m_bStickMove = false;
		}

		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || CManager::GetController()->GetJoypadTrigger(3))
		{ //Enterキーを押したとき または Bボタンを押したとき
			for (int nCount = 0; nCount < MENU_MAX; nCount++)
			{
				if (m_apMenuUi[nCount] != NULL)
				{
					if (m_apMenuUi[nCount]->GetPauseChoice() == true)
					{
						switch (nCount)
						{
						case MENU_CONTINUE:

							//シーンの更新を進める
							CScene::UpdateStop(false);

							//終了処理
							Uninit();
							return;

							break;

						case MENU_INSTRUCTION:

							m_pInstructionUi = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CUi::UI_TYPE_DESCRIPTION);

							break;

						case MENU_START_OVER:

							CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_GAME);

							if (CManager::GetModePointer() != NULL)
							{
								//ステージ番号の初期化
								CGame *pGame = dynamic_cast<CGame*>(CManager::GetModePointer());
								pGame->SetStageNum(0);
							}

							if (CManager::GetPlayer() != NULL)
							{
								if (CManager::GetPlayer()->GetPlayerLife() < PLAYER_LIFE)
								{ //ライフが減っていたとき
									//ライフの初期化
									CManager::GetPlayer()->SetLife(PLAYER_LIFE);
								}
							}

							break;

						case MENU_RETURN_TO_SELECT:

							CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_SELECT);

							break;
						}

						break;
					}
				}
			}
		}
	}
	else
	{
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_X) || CManager::GetController()->GetJoypadTrigger(CController::BUTTON_X))
		{ //Xキー または Xボタンを押したとき
			//終了処理
			m_pInstructionUi->Uninit();
			m_pInstructionUi = NULL;
		}
	}
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CPause::Draw(void)
{
}