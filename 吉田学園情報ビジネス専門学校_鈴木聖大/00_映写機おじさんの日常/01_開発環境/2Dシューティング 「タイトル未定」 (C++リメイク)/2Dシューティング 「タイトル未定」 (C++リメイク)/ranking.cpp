//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include <winsock2.h>
#include "ranking.h"
#include "manager.h"
#include "keyboard.h"
#include "controller.h"
#include "bg.h"
#include "fade.h"
#include "door.h"
#include "score.h"
#include "number.h"
#include "ui.h"
#include "sound.h"

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
CProtocolRanking::RankingData CRanking::m_aRankingData[MAX_RANKING] = {};

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CRanking::CRanking()
{
	memset(m_apScore, 0, sizeof(m_apScore));
	memset(m_apUi, 0, sizeof(m_apUi));
	m_nCntScore = 0;
	m_bUpdateScore = true;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CRanking::~CRanking()
{
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CRanking::Init(void)
{
	//BGMの再生
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM03);

	//ランキングの取得
	GetRanking();

	//背景の生成
	CBg::Create();

	//メッセージUIの生成
	CUi::Create(D3DXVECTOR3(300.0f, 100.0f, 0.0f), D3DXVECTOR3(270.0f, 100.0f, 0.0f), CUi::UI_TYPE_RANKING_MESSAGE);

	//プレイヤーの生成
	CManager::CreatePlayer();

	//ドアの生成
	CDoor::Create(D3DXVECTOR3(229.8f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_SELECT);

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CRanking::Uninit(void)
{
	//BGMの停止
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM03);

	//全てのメモリの開放処理
	CScene::ReleaseAllOtherThanFadeAndPlayer();

	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	{
		if (m_apScore[nCount] != NULL)
		{
			m_apScore[nCount] = NULL;
		}

		if (m_apUi[nCount] != NULL)
		{		
			m_apUi[nCount] = NULL;
		}
	}

	// winsock終了処理
	WSACleanup();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CRanking::Update(void)
{
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	DIJOYSTATE2 Controller = CManager::GetController()->GetController();

	D3DXVECTOR3 NumPos;
	D3DXVECTOR3 UiPos;

	if (pKeyboard->GetKeyboardTrigger(DIK_R) || CManager::GetController()->GetJoypadTrigger(1))
	{ //Rキー または Yボタンを押したとき
		//フェードの生成
		CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_RANKING);
	}

	if (m_bUpdateScore == true)
	{ //更新が必要なとき
		//キーボードの入力処理
		if (pKeyboard->GetKeyboardPress(DIK_S) || CManager::GetController()->GetJoypadPress(7))
		{ //Sキー または R2ボタンを押したとき
			//スコアを生成するカウントを進める
			m_nCntScore += 5;
		}
		else
		{
			//スコアを生成するカウントを進める
			m_nCntScore++;
		}

		if (m_nCntScore >= 120)
		{ //120フレーム経ったとき
			for (int nCount = MAX_RANKING - 1; nCount > 2; nCount--)
			{
				if (m_apScore[nCount] == NULL)
				{
					//スコアの生成(10位から)
					m_apScore[nCount] = CScore::Create(D3DXVECTOR3(900.0f, -50.0f, 0.0f), D3DXVECTOR3(80.0f, 100.0f, 0.0f),40.0f, m_aRankingData[nCount].nScore, CNumber::TYPE_1);

					for (int nCntUi = 0; nCntUi < 7; nCntUi++)
					{
						if (m_apUi[nCntUi] == NULL)
						{
							//順位UIの生成
							m_apUi[nCntUi] = CUi::Create(D3DXVECTOR3(600.0f, -50.0f, 0.0f), D3DXVECTOR3(140.0f, 100.0f, 0.0f), (CUi::UI_TYPE)(CUi::UI_TYPE_RANKING_10 - nCntUi));

							break;
						}
					}

					//カウントの初期化
					m_nCntScore = 0;

					break;
				}
			}
		}

		if (m_apScore[3] != NULL)
		{ //4位が生成されているとき
			if (m_nCntScore >= 210)
			{ //240フレーム経ったとき
				for (int nCount = 2; nCount >= 0; nCount--)
				{
					if (m_apScore[nCount] == NULL)
					{
						//スコアの生成(3位から)
						m_apScore[nCount] = CScore::Create(D3DXVECTOR3(900.0f, -50.0f, 0.0f), D3DXVECTOR3(80.0f, 100.0f, 0.0f), 40.0f, m_aRankingData[nCount].nScore, CNumber::TYPE_1);

						for (int nCntUi = 7; nCntUi < 10; nCntUi++)
						{
							if (m_apUi[nCntUi] == NULL)
							{
								//順位UIの生成
								m_apUi[nCntUi] = CUi::Create(D3DXVECTOR3(600.0f, -50.0f, 0.0f), D3DXVECTOR3(140.0f, 100.0f, 0.0f), (CUi::UI_TYPE)(CUi::UI_TYPE_RANKING_10 - nCntUi));
							
								break;
							}
						}

						//カウントの初期化
						m_nCntScore = 0;

						break;
					}
				}
			}
		}

		for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
		{
			if (m_apScore[nCntRank] != NULL)
			{
				for (int nCntNum = 0; nCntNum < SCORE_NUM; nCntNum++)
				{
					if (m_apScore[nCntRank]->GetNumber(nCntNum) != NULL)
					{
						//スコアの位置の取得
						NumPos = m_apScore[nCntRank]->GetNumber(nCntNum)->GetPos();

						if (nCntRank == 1)
						{ //2位の位置が
							if (NumPos.y >= 360.0f)
							{ //半分まで来たとき
								//スコアの更新を止める
								m_bUpdateScore = false;
								return;
							}
						}

						//キーボードの入力処理
						if (pKeyboard->GetKeyboardPress(DIK_S) || CManager::GetController()->GetJoypadPress(7))
						{ //Sキー または R2ボタンを押したとき
							//スコアの位置の更新
							NumPos.y += 5.0f;
						}
						else
						{
							//スコアの位置の更新
							NumPos.y += 1.0f;
						}
						m_apScore[nCntRank]->GetNumber(nCntNum)->SetPos(NumPos);

						if (NumPos.y - 50.0f > SCREEN_HEIGHT)
						{ //スコアが画面外に出たとき
							//スコアの終了処理
							m_apScore[nCntRank]->Uninit();
						}
						else if (m_apScore[nCntRank] != NULL)
						{
							//スコアの更新処理
							m_apScore[nCntRank]->Update();
						}
					}
				}
			}

			if (m_apUi[nCntRank] != NULL)
			{
				//順位UIの位置の取得
				UiPos = m_apUi[nCntRank]->GetPos();

				//キーボードの入力処理
				if (pKeyboard->GetKeyboardPress(DIK_S) || CManager::GetController()->GetJoypadPress(7))
				{ //Sキー または R2ボタンを押したとき
					//順位UIの位置の更新
					UiPos.y += 5.0f;
				}
				else
				{
					//順位UIの位置の更新
					UiPos.y += 1.0f;
				}

				m_apUi[nCntRank]->SetPos(UiPos);

				if (UiPos.y - 50.0f > SCREEN_HEIGHT)
				{ //順位UIが画面外に出たとき
					//順位UIの終了処理
					m_apUi[nCntRank]->Uninit();
					m_apUi[nCntRank] = NULL;
				}

				if (m_apUi[nCntRank] != NULL)
				{
					//順位UIの更新処理
					m_apUi[nCntRank]->Update();
				}
			}
		}
	}
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CRanking::Draw(void)
{
}

//*****************************************************************************
// ランキングの設定
//*****************************************************************************
int CRanking::SetRanking(int nScore)
{
	WSADATA wsaData;
	char aDestlPAddr[BUFFER_NUM] = "127.0.0.1";

	// winsock初期化処理
	WSAStartup(WINSOCK_VERSION, &wsaData);

	//メモリの確保
	CTcpClient *m_pTcpClient = NULL;
	m_pTcpClient = m_pTcpClient->Create(IP_ADDRESS, SERVER_PORT_NUM);

	if (m_pTcpClient == NULL)
	{
		// winsock終了処理
		WSACleanup();
	}

	char aSendBuf[BUFFER_NUM];

	memset(aSendBuf, 0, sizeof(aSendBuf));

	//1番目の配列に２を入れる
	aSendBuf[0] = CProtocolRanking::COMMAND_TYPE_SET_RANKING;

	//エンディアン変換
	nScore = htonl(nScore);

	//2番目の配列から4バイト分のTimeデータを格納(Time(int型)は4バイトだから)
	memcpy(&aSendBuf[sizeof(aSendBuf[0])], &nScore, sizeof(nScore));

	//ランキング設定リクエストの送信
	m_pTcpClient->Send(aSendBuf, sizeof(aSendBuf[0]) + sizeof(nScore));

	char aRecvBuf[BUFFER_NUM];
	memset(aRecvBuf, 0, sizeof(aRecvBuf));

	//ランキング順位を受信
	m_pTcpClient->Recv(aRecvBuf, sizeof(aRecvBuf[0]));

	//クライアントの終了処理
	m_pTcpClient->Release();

	return aRecvBuf[0];
}

//*****************************************************************************
// ランキングの取得
//*****************************************************************************
void CRanking::GetRanking(void)
{
	WSADATA wsaData;
	char aDestlPAddr[BUFFER_NUM] = "127.0.0.1";

	//winsock初期化処理
	WSAStartup(WINSOCK_VERSION, &wsaData);

	//メモリの確保
	CTcpClient *m_pTcpClient = NULL;
	m_pTcpClient = m_pTcpClient->Create(IP_ADDRESS, SERVER_PORT_NUM);

	if (m_pTcpClient == NULL)
	{
		//winsock終了処理
		WSACleanup();
	}

	char aSendBuf[BUFFER_NUM];
	memset(aSendBuf, 0, sizeof(aSendBuf));
	aSendBuf[0] = CProtocolRanking::COMMAND_TYPE_GET_RANKING;

	//ランキング一覧取得リクエストの送信
	m_pTcpClient->Send(&aSendBuf[0], sizeof(aSendBuf[0]));	//最初の１バイトを送りたいから１バイトを指定

	//ランキング一覧取得レスポンス受信
	char aRecvBuf[BUFFER_NUM];
	memset(aRecvBuf, 0, sizeof(aRecvBuf));

	//受信処理
	m_pTcpClient->Recv(aRecvBuf, sizeof(aRecvBuf));

	int score = 0;

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		memcpy(&score, &aRecvBuf[nCntRank * sizeof(int)], sizeof(int));
		score = ntohl(score);
		m_aRankingData[nCntRank].nScore = score;
	}

	//クライアントの終了処理
	m_pTcpClient->Release();
}