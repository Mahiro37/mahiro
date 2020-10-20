//=============================================================================
//
// スタート時のカウント処理 [start_count.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "start_count.h"
#include "number.h"
#include "ui.h"
#include "manager.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define COUNT_SPEED		(60)	//カウントのスピード
#define START_COUNT		(3)		//最初の数字

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CStartCount::CStartCount()
{
	m_pNumber = NULL;
	m_pUi = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nStartNumber = START_COUNT;
	m_nCounter = 0;
	m_nCntStageNum = 0;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CStartCount::~CStartCount()
{
}

//*****************************************************************************
//スタートカウントの生成
//*****************************************************************************
CStartCount *CStartCount::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//スタートカウントのメモリ確保
	CStartCount *pStartCount = new CStartCount;

	//スタートカウントの初期化処理
	if (pStartCount != NULL)
	{
		pStartCount->Init(pos, size);
	}

	return pStartCount;
}

HRESULT CStartCount::Init(void)
{
	return S_OK;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CStartCount::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	//オブジェクトの種類の設定
	SetObjType(OBJTYPE_START_OR_FINISH);

	//シーンの更新を止める
	UpdateStop(true);

	if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
	{
		//数字の生成
		m_pNumber = CNumber::Create(pos, size, 3, CNumber::TYPE_1);

		if (m_pNumber != NULL)
		{
			//数字の設定処理
			m_pNumber->SetNumber(m_nStartNumber);

			//BGMの再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_COUNT);
		}
	}
	else if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (m_pUi == NULL)
		{
			//ステージ番号のUIの生成
			m_pUi = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(450.0f, 200.0f, 0.0f), CUi::UI_TYPE_STAGE_1);

			//BGMの再生
			CManager::GetSound()->Play(CSound::SOUND_LABEL_STAGE_NUM);
		}
	}

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CStartCount::Uninit(void)
{
	//数字の終了処理
	if (m_pNumber != NULL)
	{
		m_pNumber->Uninit();
		delete m_pNumber;
		m_pNumber = NULL;
	}

	if (m_pUi != NULL)
	{
		//m_pUi->Uninit();
		m_pUi = NULL;
	}

	//メモリの開放処理
	Release();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CStartCount::Update(void)
{
	//カウントのカウントを進める
	m_nCounter++;

	switch (CManager::GetMode())
	{
	case CManager::MODE_SCORE_ATTACK:

		if (m_nCounter == COUNT_SPEED)
		{
			m_nCounter = 0;

			m_nStartNumber--;

			if (m_nStartNumber >= 0)
			{
				if (m_nStartNumber != 0)
				{
					//数字の更新処理
					if (m_pNumber != NULL)
					{
						m_pNumber->SetNumber(m_nStartNumber);
					}
				}
				else
				{
					//カウントの終了処理
					if (m_pNumber != NULL)
					{
						m_pNumber->Uninit();
						delete m_pNumber;
						m_pNumber = NULL;
					}

					//StartUIの生成
					if (m_pUi == NULL)
					{
						m_pUi = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(400.0f, 200.0f, 0.0f), CUi::UI_TYPE_START);
					}
				}
			}
			else
			{
				//BGMの再生
				CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM05);

				if (m_pUi != NULL)
				{
					//StartUIの終了処理
					m_pUi->Uninit();
					m_pUi = NULL;
				}

				//シーンの更新を進める
				UpdateStop(false);

				//終了処理
				Uninit();
				return;
			}
		}

		break;

	case CManager::MODE_GAME:

		if (m_pUi != NULL)
		{
			if (m_nStartNumber == 3)
			{
				if (CManager::GetModePointer() != NULL)
				{
					//テクスチャの更新
					CGame *pGame = dynamic_cast<CGame*>(CManager::GetModePointer());
					m_pUi->BindTexture((CUi::UI_TYPE)(CUi::UI_TYPE_STAGE_1 + pGame->GetStageNum()));
				}
			}
		}

		if (m_nCounter == COUNT_SPEED)
		{
			m_nCounter = 0;

			if (m_pNumber == NULL)
			{
				if (m_pUi != NULL)
				{
					if (m_nStartNumber == 3)
					{
						m_nCntStageNum++;

						if (m_nCntStageNum == 2)
						{
							//ステージ番号のUIの終了処理
							if (m_pUi != NULL)
							{
								m_pUi->Uninit();
								m_pUi = NULL;
							}
						}
						else
						{
							return;
						}
					}
				}
			}
			else
			{
				m_nStartNumber--;
			}

			if (m_nStartNumber >= 0)
			{
				if (m_nStartNumber > 0)
				{
					if (m_pNumber == NULL)
					{
						//数字の生成
						m_pNumber = CNumber::Create(m_pos, m_size, m_nStartNumber, CNumber::TYPE_1);

						//ステージ番号のUIの終了処理
						if (m_pUi != NULL)
						{
							m_pUi->Uninit();
							m_pUi = NULL;
						}

						//BGMの再生
						CManager::GetSound()->Play(CSound::SOUND_LABEL_COUNT);

						break;
					}
					else
					{
						if (m_nStartNumber > 0)
						{
							//数字の更新処理
							m_pNumber->SetNumber(m_nStartNumber);
						}
					}
				}
				else
				{
					//カウントの終了処理
					if (m_pNumber != NULL)
					{
						m_pNumber->Uninit();
						delete m_pNumber;
						m_pNumber = NULL;
					}

					//StartUIの生成
					if (m_pUi == NULL)
					{
						m_pUi = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(400.0f, 200.0f, 0.0f), CUi::UI_TYPE_START);
					}

					m_nStartNumber--;
				}
			}
			else
			{
				//BGMの再生
				CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM05);

				if (m_pUi != NULL)
				{
					//StartUIの終了処理
					m_pUi->Uninit();
					m_pUi = NULL;
				}

				//シーンの更新を進める
				UpdateStop(false);

				//終了処理
				Uninit();
				return;
			}
		}

		break;

	default:

		//カウントの初期化
		m_nCounter = 0;

		break;
	}

	//数字の更新処理
	if (m_pNumber != NULL)
	{
		m_pNumber->Update();
	}
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CStartCount::Draw(void)
{
	//数字の描画処理
	if (m_pNumber != NULL)
	{
		m_pNumber->Draw();
	}
}