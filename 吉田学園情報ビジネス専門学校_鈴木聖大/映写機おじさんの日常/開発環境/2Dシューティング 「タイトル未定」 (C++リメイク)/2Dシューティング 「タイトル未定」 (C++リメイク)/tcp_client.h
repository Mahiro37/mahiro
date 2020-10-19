//=============================================================================
//
// クライアント処理 [tcp_client.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib,"ws2_32.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SERVER_PORT_NUM (12345)	// 接続先サーバーの接続先ポート
#define BUFFER_NUM		(128)	// 送受信バッファーサイズ

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();
	CTcpClient *Create(const char *pHostName, int nPortNum);
	bool Init(const char *pHostName, int nPortNum);
	int Send(char *pSendData, int nSendDataSize);
	int Recv(char *pRecvBuf, int nRecvBufSize);
	void Close(void);
	void Release(void);

private:
	SOCKET m_sock;
};

#endif