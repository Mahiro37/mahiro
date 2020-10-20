//=============================================================================
//
// サーバー処理 [main.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include <winsock2.h>
#include <stdio.h>
#include <thread>
#include "protocol_ranking.h"
#include "tcp_listener.h"
#include "tcp_client.h"

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
void ReadFile(void);
void WriteFile(void);
void WorkerThread(CTcpClient *pTcpClient);

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_RANKING		(10)			// ランキングデータ数
#define SERVER_PORT_NUM (12345)			//ポート番号
#define MAX_BUFFER		(128)			//バッファの最大数

//*****************************************************************************
//構造体定義
//*****************************************************************************
typedef struct
{
	int nScore;				//記録スコア
}RECVDATA;

//*****************************************************************************
//グローバル変数
//*****************************************************************************
RankingData g_aRankingData[MAX_RANKING];		//ランキングのデータ

//*****************************************************************************
//メイン関数
//*****************************************************************************
void main(void)
{
	WSADATA wsaData;

	//winsock初期化処理
	WSAStartup(WINSOCK_VERSION, &wsaData);

	//グローバル変数の初期化
	memset(g_aRankingData, 0, sizeof(g_aRankingData));

	//ランキングファイルを読み込む
	ReadFile();

	//CTcpListener生成
	CTcpListener *pTcpListener = CTcpListener::Create(SERVER_PORT_NUM);

	while (1)
	{
		//接続待ち
		CTcpClient *pTcpClient = pTcpListener->Accept();

		//接続できなかったとき
		if (pTcpClient == NULL)
		{
			continue;	//Acceptをやり直す
		}

		//スレッド生成
		std::thread thWorker(WorkerThread, pTcpClient);

		//スレッドを切り離す
		thWorker.detach();
	}

	//CTcpListenerを破棄
	pTcpListener->Release();

	//winsock終了処理
	WSACleanup();
}

//*****************************************************************************
//読み込み処理
//*****************************************************************************
void ReadFile(void)
{
	FILE *pFile;

	//ファイルを開く
	pFile = fopen("ranking.txt", "r");

	//ファイルが開けたとき
	if (pFile != NULL)
	{
		//MAX_RANKING分のタイムと名前を保存
		for (int nCount = 0; nCount < MAX_RANKING; nCount++)
		{
			fscanf(pFile, "%d\n", &g_aRankingData[nCount].nScore);
		}
	}
	//ファイルが開けなかったとき
	else
	{
		printf("ファイルを開けませんでした\n");
	}

	//ファイルを閉じる
	fclose(pFile);
}

//*****************************************************************************
//書き込み処理
//*****************************************************************************
void WriteFile(void)
{
	FILE *pFile;

	//ファイルを開く
	pFile = fopen("ranking.txt", "w");

	//ファイルが開けたとき
	if (pFile != NULL)
	{
		//MAX_RANKING分のタイムと名前を保存
		for (int nCount = 0; nCount < MAX_RANKING; nCount++)
		{
			fprintf(pFile, "%d\n", g_aRankingData[nCount].nScore);
		}
	}
	//ファイルが開けなかったとき
	else
	{
		printf("ファイルを開けませんでした\n");
	}

	//ファイルを閉じる
	fclose(pFile);
}

//*****************************************************************************
//クライアントとの送受信処理
//*****************************************************************************
void WorkerThread(CTcpClient *pTcpClient)
{
	RECVDATA RecvData;
	char aSendBuff[MAX_BUFFER];		//送信内容
	char aRecvBuff[MAX_BUFFER];		//受信内容
	int nRecvNum = 0;				//受信判定
	int nTime = 0;					//エンディアン判定をしたnScore
	int nCntRank = 0;				//順位

	//送信バッファのクリア
	memset(aSendBuff, 0, sizeof(aSendBuff));

	//受信バッファのクリア
	memset(aRecvBuff, 0, sizeof(aRecvBuff));

	//受信処理
	nRecvNum = pTcpClient->Recv(aRecvBuff, sizeof(aRecvBuff));

	//1バイト目の処理情報を取得
	COMMAND_TYPE command = (COMMAND_TYPE)aRecvBuff[0];

	switch (command)
	{
	case COMMAND_TYPE_GET_RANKING:	//ランキング一覧を送信

		//ランキングデータをaSendBuffにコピー
		for (int nCount = 0; nCount < MAX_RANKING; nCount++)
		{
			nTime = htonl(g_aRankingData[nCount].nScore);
			memcpy(&aSendBuff[nCount * sizeof(int)], &nTime, sizeof(nTime));
		}

		//送信処理
		pTcpClient->Send(aSendBuff, sizeof(int) * MAX_RANKING);

		break;

	case COMMAND_TYPE_SET_RANKING:	//ランキングに設定

		//受信した記録タイムを取得
		memcpy(&RecvData.nScore, &aRecvBuff[sizeof(aRecvBuff[0])], sizeof(int));

		//記録タイムをエンディアン変換
		RecvData.nScore = ntohl(RecvData.nScore);

		//ランキング内に入るか確認
		for (nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
		{
			//(nCount+1)位より早かったとき
			if (g_aRankingData[nCntRank].nScore == 0 || g_aRankingData[nCntRank].nScore <= RecvData.nScore)
			{
				for (int nCount = 8; nCount >= nCntRank; nCount--)
				{
					//順位を1つずつ下げる
					g_aRankingData[nCount + 1].nScore = g_aRankingData[nCount].nScore;
				}

				//空いたところに記録タイムを入れる
				g_aRankingData[nCntRank].nScore = RecvData.nScore;

				//ランキングファイルに書き込む
				WriteFile();

				break;
			}
		}

		//送信処理
		aSendBuff[0] = nCntRank + 1;
		pTcpClient->Send(&aSendBuff[0], sizeof(aSendBuff[0]));

		break;

	default:	//どれにも当てはまらなかったとき

		break;
	}

	//pTcpClientを破棄
	pTcpClient->Release();
}