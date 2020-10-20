//=========================================================
//
//�N���C�A���g�p�v���W�F�N�g
//Author:Suzuki Mahiro
//
//=========================================================
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

//*********************************************************
//���C�u����
//*********************************************************
#pragma comment(lib,"ws2_32.lib")

//*********************************************************
//�N���X��`
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