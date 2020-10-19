//=============================================================================
//
// �N���C�A���g���� [tcp_client.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment(lib,"ws2_32.lib")

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SERVER_PORT_NUM (12345)	// �ڑ���T�[�o�[�̐ڑ���|�[�g
#define BUFFER_NUM		(128)	// ����M�o�b�t�@�[�T�C�Y

//*****************************************************************************
//�N���X��`
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