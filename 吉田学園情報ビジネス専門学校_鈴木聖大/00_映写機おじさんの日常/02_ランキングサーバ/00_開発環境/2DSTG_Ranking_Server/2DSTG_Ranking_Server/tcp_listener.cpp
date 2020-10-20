//=============================================================================
//
// �T�[�o�[���� [tcp_listener.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "tcp_listener.h"
#include "tcp_client.h"
#include <thread>			//�X���b�h

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_ACCEPT_CONN (100)		// �ڑ���t�ő吔

//*****************************************************************************
//CTcpListener�R���X�g���N�^
//*****************************************************************************
CTcpListener::CTcpListener()
{
}

//*****************************************************************************
//CTcpListener�f�X�g���N�^
//*****************************************************************************
CTcpListener::~CTcpListener()
{
}

//*****************************************************************************
//CTcpListener����
//*****************************************************************************
CTcpListener* CTcpListener::Create(int nPortNum)
{
	//�������̊m��
	CTcpListener *pTcpListener = new CTcpListener;// TCP�ڑ���t�N���X�̃C���X�^���X����

	if (pTcpListener->Init(nPortNum, MAX_ACCEPT_CONN) == false)
	{
		delete pTcpListener;
		pTcpListener = NULL;
	}

	return pTcpListener;
}

//*****************************************************************************
//����������
//*****************************************************************************
bool CTcpListener::Init(int nPortNum, int nAcceptConnNum)
{
	struct sockaddr_in addr;

	// �ڑ���t�p�\�P�b�g�̐��� (socket)
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	// �ڑ���t���̐ݒ�
	addr.sin_family = AF_INET;     // IPv4
	addr.sin_port = htons(nPortNum);  // �|�[�g�ԍ�
	addr.sin_addr.S_un.S_addr = INADDR_ANY;  // ���ׂĂ̐ڑ����󂯓����

	// �\�P�b�g��IP�A�h���X�ƃ|�[�g�ԍ���ݒ肷�� (bind)
	bind(m_sockServer, (struct sockaddr*) &addr, sizeof(addr));

	// �ڑ��v����t���J�n���� (listen)
	listen(m_sockServer, nAcceptConnNum);

	return true;
}

//*****************************************************************************
//�ڑ���t����
//*****************************************************************************
CTcpClient* CTcpListener::Accept()
{
	struct sockaddr_in client;
	int nAddrLength = 0;

	if (m_sockServer < 0)
	{
		return NULL;
	}

	// �\�P�b�g�ڑ����󂯕t����
	nAddrLength = sizeof(client);

	SOCKET sock = accept(m_sockServer, (struct sockaddr*) &client, &nAddrLength);
	CTcpClient *pClient = CTcpClient::Create(sock);

	return pClient;
}

//*****************************************************************************
//�j������
//*****************************************************************************
void CTcpListener::Release()
{
	// �ؒf����
	CTcpListener::Close();

	// �C���X�^���X�폜
	delete this;
}

//*****************************************************************************
//�N���[�Y����
//*****************************************************************************
void CTcpListener::Close()
{
	if (m_sockServer < 0)
	{
		return;
	}

	// �\�P�b�g�ڑ����N���[�Y
	closesocket(m_sockServer);
}