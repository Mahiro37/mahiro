//=============================================================================
//
// �N���C�A���g [tcp_client.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include <winsock2.h>
#include <stdio.h>
#include "tcp_client.h"

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CTcpClient::CTcpClient()
{
	m_sock = -1;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CTcpClient::~CTcpClient()
{
}

//*****************************************************************************
//�N���C�A���g�̐���
//*****************************************************************************
CTcpClient *CTcpClient::Create(const char *pHostName, int nPortNum)
{
	//�������̊m��
	CTcpClient *pTcpClient = new CTcpClient;

	if (pTcpClient->Init(pHostName, nPortNum) == false)
	{
		delete pTcpClient;
		pTcpClient = NULL;
	}

	return pTcpClient;
}

//*****************************************************************************
//�N���C�A���g�̐���
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
//����������
//*****************************************************************************
bool CTcpClient::Init(const char *pHostName, int nPortNum)
{
	// �ڑ��p�\�P�b�g�̐���
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sock == INVALID_SOCKET)
	{
		printf("socket:%d\n", WSAGetLastError()); // �G���[����
		return 1;
	}

	// �ڑ�����̐ݒ�
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPortNum);
	addr.sin_addr.S_un.S_addr = inet_addr(pHostName);

	// �T�[�o�[�ɐڑ�����
	if (connect(m_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		printf("connect:%d\n", WSAGetLastError());

		//�ؒf����
		Close(m_sock);

		return false;
	}    // �T�[�o�[�ɐڑ��ł���(�T�[�o�[��accept�͎��̏��������s)

	return true;
}

//*****************************************************************************
//����������
//*****************************************************************************
bool CTcpClient::Init(SOCKET sock)
{
	m_sock = sock;

	return true;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CTcpClient::Release(void)
{
	//�ؒf����
	Close(m_sock);

	delete this;
}

//*****************************************************************************
//�ؒf����
//*****************************************************************************
void CTcpClient::Close(SOCKET sock)
{
	if (sock < 0)
	{
		return;
	}

	// �\�P�b�g�ڑ����N���[�Y
	closesocket(sock);

	sock -= 1;
}

//*****************************************************************************
//���M����
//*****************************************************************************
int CTcpClient::Send(char *pSendData, int nSendDataSize)
{
	if (m_sock < 0)
	{
		return 0;
	}

	//���M
	int nSendSize = send(m_sock, pSendData, nSendDataSize, 0);//�����񕪂̗e�ʂ��擾(strlen)

	return nSendSize;
}

//*****************************************************************************
//��M����
//*****************************************************************************
int CTcpClient::Recv(char *pRecvBuf, int nRecvBufSize)
{
	if (m_sock < 0)
	{
		return 0;
	}

	// �f�[�^����M����
	int nRecvSize = recv(m_sock, pRecvBuf, nRecvBufSize, 0);

	// �u�I���v�Ɠ��́A�������́A��M�����f�[�^�T�C�Y��0��������T�[�o�[�Ƃ̐ڑ���ؒf����
	if (nRecvSize <= 0)
	{
		printf("�f�[�^����M�ł��܂���ł���\n");

		// �\�P�b�g�ڑ����N���[�Y
		closesocket(m_sock);
	}

	return nRecvSize;
}