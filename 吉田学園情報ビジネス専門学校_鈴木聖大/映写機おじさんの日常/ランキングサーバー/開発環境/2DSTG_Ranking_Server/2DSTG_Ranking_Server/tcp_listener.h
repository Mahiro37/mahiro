//========================
//サーバー用プロジェクト
//Author:Suzuki Mahiro
//========================
#ifndef _TCPLISTENER_H_
#define _TCPLISTENER_H_

#include <winsock2.h>

//*********************************************************
// 前方宣言
//*********************************************************
class CTcpClient;

//*********************************************************
// TCP接続受付クラス
//*********************************************************
class CTcpListener
{
public:
	CTcpListener();
	~CTcpListener();
	bool Init(int nPortNum, int nAcceptConnNum);
	CTcpClient* CTcpListener::Accept(void);
	void Release(void);
	void Close(void);
	static CTcpListener *Create(int nPortNum);

private:
	SOCKET m_sockServer;
};

#endif