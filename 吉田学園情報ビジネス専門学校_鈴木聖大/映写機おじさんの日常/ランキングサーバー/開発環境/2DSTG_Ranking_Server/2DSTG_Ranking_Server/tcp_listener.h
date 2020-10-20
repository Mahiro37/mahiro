//========================
//�T�[�o�[�p�v���W�F�N�g
//Author:Suzuki Mahiro
//========================
#ifndef _TCPLISTENER_H_
#define _TCPLISTENER_H_

#include <winsock2.h>

//*********************************************************
// �O���錾
//*********************************************************
class CTcpClient;

//*********************************************************
// TCP�ڑ���t�N���X
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