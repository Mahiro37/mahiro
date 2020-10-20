//=========================================================
//
//クライアント用プロジェクト
//Author:Suzuki Mahiro
//
//=========================================================
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

//*********************************************************
//ライブラリ
//*********************************************************
#pragma comment(lib,"ws2_32.lib")

//*********************************************************
//クラス定義
//*********************************************************
class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();
	bool Init(const char *pHostName, int nPortNum);
	int Send(char *pSendData, int nSendDataSize);
	int Recv(char *pRecvBuf, int nRecvBufSize);
	static void Close(SOCKET sock);
	void Release(void);
	CTcpClient *Create(const char *pHostName, int nPortNum);

	static CTcpClient *Create(SOCKET sock);
	bool Init(SOCKET sock);

private:
	SOCKET m_sock;
};

#endif