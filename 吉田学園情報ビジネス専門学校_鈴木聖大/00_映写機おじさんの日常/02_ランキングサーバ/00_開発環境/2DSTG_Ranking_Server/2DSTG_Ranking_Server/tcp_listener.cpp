//=============================================================================
//
// サーバー処理 [tcp_listener.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "tcp_listener.h"
#include "tcp_client.h"
#include <thread>			//スレッド

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_ACCEPT_CONN (100)		// 接続受付最大数

//*****************************************************************************
//CTcpListenerコンストラクタ
//*****************************************************************************
CTcpListener::CTcpListener()
{
}

//*****************************************************************************
//CTcpListenerデストラクタ
//*****************************************************************************
CTcpListener::~CTcpListener()
{
}

//*****************************************************************************
//CTcpListener生成
//*****************************************************************************
CTcpListener* CTcpListener::Create(int nPortNum)
{
	//メモリの確保
	CTcpListener *pTcpListener = new CTcpListener;// TCP接続受付クラスのインスタンス生成

	if (pTcpListener->Init(nPortNum, MAX_ACCEPT_CONN) == false)
	{
		delete pTcpListener;
		pTcpListener = NULL;
	}

	return pTcpListener;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
bool CTcpListener::Init(int nPortNum, int nAcceptConnNum)
{
	struct sockaddr_in addr;

	// 接続受付用ソケットの生成 (socket)
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	// 接続受付情報の設定
	addr.sin_family = AF_INET;     // IPv4
	addr.sin_port = htons(nPortNum);  // ポート番号
	addr.sin_addr.S_un.S_addr = INADDR_ANY;  // すべての接続を受け入れる

	// ソケットにIPアドレスとポート番号を設定する (bind)
	bind(m_sockServer, (struct sockaddr*) &addr, sizeof(addr));

	// 接続要求受付を開始する (listen)
	listen(m_sockServer, nAcceptConnNum);

	return true;
}

//*****************************************************************************
//接続受付処理
//*****************************************************************************
CTcpClient* CTcpListener::Accept()
{
	struct sockaddr_in client;
	int nAddrLength = 0;

	if (m_sockServer < 0)
	{
		return NULL;
	}

	// ソケット接続を受け付ける
	nAddrLength = sizeof(client);

	SOCKET sock = accept(m_sockServer, (struct sockaddr*) &client, &nAddrLength);
	CTcpClient *pClient = CTcpClient::Create(sock);

	return pClient;
}

//*****************************************************************************
//破棄処理
//*****************************************************************************
void CTcpListener::Release()
{
	// 切断処理
	CTcpListener::Close();

	// インスタンス削除
	delete this;
}

//*****************************************************************************
//クローズ処理
//*****************************************************************************
void CTcpListener::Close()
{
	if (m_sockServer < 0)
	{
		return;
	}

	// ソケット接続をクローズ
	closesocket(m_sockServer);
}