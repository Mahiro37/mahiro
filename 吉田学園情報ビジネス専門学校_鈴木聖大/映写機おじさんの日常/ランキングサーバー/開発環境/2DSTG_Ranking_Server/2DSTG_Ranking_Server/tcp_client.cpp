//=============================================================================
//
// クライアント [tcp_client.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include <winsock2.h>
#include <stdio.h>
#include "tcp_client.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CTcpClient::CTcpClient()
{
	m_sock = -1;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CTcpClient::~CTcpClient()
{
}

//*****************************************************************************
//クライアントの生成
//*****************************************************************************
CTcpClient *CTcpClient::Create(const char *pHostName, int nPortNum)
{
	//メモリの確保
	CTcpClient *pTcpClient = new CTcpClient;

	if (pTcpClient->Init(pHostName, nPortNum) == false)
	{
		delete pTcpClient;
		pTcpClient = NULL;
	}

	return pTcpClient;
}

//*****************************************************************************
//クライアントの生成
//*****************************************************************************
CTcpClient *CTcpClient::Create(SOCKET sock)
{
	CTcpClient *client = new CTcpClient;

	if (client->Init(sock) == false)
	{
		delete client;
		client = NULL;
	}

	return client;
}


//*****************************************************************************
//初期化処理
//*****************************************************************************
bool CTcpClient::Init(const char *pHostName, int nPortNum)
{
	// 接続用ソケットの生成
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sock == INVALID_SOCKET)
	{
		printf("socket:%d\n", WSAGetLastError()); // エラー判定
		return 1;
	}

	// 接続先情報の設定
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPortNum);
	addr.sin_addr.S_un.S_addr = inet_addr(pHostName);

	// サーバーに接続する
	if (connect(m_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		printf("connect:%d\n", WSAGetLastError());

		//切断処理
		Close(m_sock);

		return false;
	}    // サーバーに接続できた(サーバーはacceptは次の処理を実行)

	return true;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
bool CTcpClient::Init(SOCKET sock)
{
	m_sock = sock;

	return true;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CTcpClient::Release(void)
{
	//切断処理
	Close(m_sock);

	delete this;
}

//*****************************************************************************
//切断処理
//*****************************************************************************
void CTcpClient::Close(SOCKET sock)
{
	if (sock < 0)
	{
		return;
	}

	// ソケット接続をクローズ
	closesocket(sock);

	sock -= 1;
}

//*****************************************************************************
//送信処理
//*****************************************************************************
int CTcpClient::Send(char *pSendData, int nSendDataSize)
{
	if (m_sock < 0)
	{
		return 0;
	}

	//送信
	int nSendSize = send(m_sock, pSendData, nSendDataSize, 0);//文字列分の容量を取得(strlen)

	return nSendSize;
}

//*****************************************************************************
//受信処理
//*****************************************************************************
int CTcpClient::Recv(char *pRecvBuf, int nRecvBufSize)
{
	if (m_sock < 0)
	{
		return 0;
	}

	// データを受信する
	int nRecvSize = recv(m_sock, pRecvBuf, nRecvBufSize, 0);

	// 「終了」と入力、もしくは、受信したデータサイズが0だったらサーバーとの接続を切断する
	if (nRecvSize <= 0)
	{
		printf("データを受信できませんでした\n");

		// ソケット接続をクローズ
		closesocket(m_sock);
	}

	return nRecvSize;
}