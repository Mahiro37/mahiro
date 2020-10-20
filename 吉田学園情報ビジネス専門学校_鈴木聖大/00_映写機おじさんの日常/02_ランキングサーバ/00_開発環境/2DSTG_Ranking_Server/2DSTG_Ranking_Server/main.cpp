//=============================================================================
//
// �T�[�o�[���� [main.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include <winsock2.h>
#include <stdio.h>
#include <thread>
#include "protocol_ranking.h"
#include "tcp_listener.h"
#include "tcp_client.h"

//*****************************************************************************
//�v���g�^�C�v�錾
//*****************************************************************************
void ReadFile(void);
void WriteFile(void);
void WorkerThread(CTcpClient *pTcpClient);

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_RANKING		(10)			// �����L���O�f�[�^��
#define SERVER_PORT_NUM (12345)			//�|�[�g�ԍ�
#define MAX_BUFFER		(128)			//�o�b�t�@�̍ő吔

//*****************************************************************************
//�\���̒�`
//*****************************************************************************
typedef struct
{
	int nScore;				//�L�^�X�R�A
}RECVDATA;

//*****************************************************************************
//�O���[�o���ϐ�
//*****************************************************************************
RankingData g_aRankingData[MAX_RANKING];		//�����L���O�̃f�[�^

//*****************************************************************************
//���C���֐�
//*****************************************************************************
void main(void)
{
	WSADATA wsaData;

	//winsock����������
	WSAStartup(WINSOCK_VERSION, &wsaData);

	//�O���[�o���ϐ��̏�����
	memset(g_aRankingData, 0, sizeof(g_aRankingData));

	//�����L���O�t�@�C����ǂݍ���
	ReadFile();

	//CTcpListener����
	CTcpListener *pTcpListener = CTcpListener::Create(SERVER_PORT_NUM);

	while (1)
	{
		//�ڑ��҂�
		CTcpClient *pTcpClient = pTcpListener->Accept();

		//�ڑ��ł��Ȃ������Ƃ�
		if (pTcpClient == NULL)
		{
			continue;	//Accept����蒼��
		}

		//�X���b�h����
		std::thread thWorker(WorkerThread, pTcpClient);

		//�X���b�h��؂藣��
		thWorker.detach();
	}

	//CTcpListener��j��
	pTcpListener->Release();

	//winsock�I������
	WSACleanup();
}

//*****************************************************************************
//�ǂݍ��ݏ���
//*****************************************************************************
void ReadFile(void)
{
	FILE *pFile;

	//�t�@�C�����J��
	pFile = fopen("ranking.txt", "r");

	//�t�@�C�����J�����Ƃ�
	if (pFile != NULL)
	{
		//MAX_RANKING���̃^�C���Ɩ��O��ۑ�
		for (int nCount = 0; nCount < MAX_RANKING; nCount++)
		{
			fscanf(pFile, "%d\n", &g_aRankingData[nCount].nScore);
		}
	}
	//�t�@�C�����J���Ȃ������Ƃ�
	else
	{
		printf("�t�@�C�����J���܂���ł���\n");
	}

	//�t�@�C�������
	fclose(pFile);
}

//*****************************************************************************
//�������ݏ���
//*****************************************************************************
void WriteFile(void)
{
	FILE *pFile;

	//�t�@�C�����J��
	pFile = fopen("ranking.txt", "w");

	//�t�@�C�����J�����Ƃ�
	if (pFile != NULL)
	{
		//MAX_RANKING���̃^�C���Ɩ��O��ۑ�
		for (int nCount = 0; nCount < MAX_RANKING; nCount++)
		{
			fprintf(pFile, "%d\n", g_aRankingData[nCount].nScore);
		}
	}
	//�t�@�C�����J���Ȃ������Ƃ�
	else
	{
		printf("�t�@�C�����J���܂���ł���\n");
	}

	//�t�@�C�������
	fclose(pFile);
}

//*****************************************************************************
//�N���C�A���g�Ƃ̑���M����
//*****************************************************************************
void WorkerThread(CTcpClient *pTcpClient)
{
	RECVDATA RecvData;
	char aSendBuff[MAX_BUFFER];		//���M���e
	char aRecvBuff[MAX_BUFFER];		//��M���e
	int nRecvNum = 0;				//��M����
	int nTime = 0;					//�G���f�B�A�����������nScore
	int nCntRank = 0;				//����

	//���M�o�b�t�@�̃N���A
	memset(aSendBuff, 0, sizeof(aSendBuff));

	//��M�o�b�t�@�̃N���A
	memset(aRecvBuff, 0, sizeof(aRecvBuff));

	//��M����
	nRecvNum = pTcpClient->Recv(aRecvBuff, sizeof(aRecvBuff));

	//1�o�C�g�ڂ̏��������擾
	COMMAND_TYPE command = (COMMAND_TYPE)aRecvBuff[0];

	switch (command)
	{
	case COMMAND_TYPE_GET_RANKING:	//�����L���O�ꗗ�𑗐M

		//�����L���O�f�[�^��aSendBuff�ɃR�s�[
		for (int nCount = 0; nCount < MAX_RANKING; nCount++)
		{
			nTime = htonl(g_aRankingData[nCount].nScore);
			memcpy(&aSendBuff[nCount * sizeof(int)], &nTime, sizeof(nTime));
		}

		//���M����
		pTcpClient->Send(aSendBuff, sizeof(int) * MAX_RANKING);

		break;

	case COMMAND_TYPE_SET_RANKING:	//�����L���O�ɐݒ�

		//��M�����L�^�^�C�����擾
		memcpy(&RecvData.nScore, &aRecvBuff[sizeof(aRecvBuff[0])], sizeof(int));

		//�L�^�^�C�����G���f�B�A���ϊ�
		RecvData.nScore = ntohl(RecvData.nScore);

		//�����L���O���ɓ��邩�m�F
		for (nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
		{
			//(nCount+1)�ʂ�葁�������Ƃ�
			if (g_aRankingData[nCntRank].nScore == 0 || g_aRankingData[nCntRank].nScore <= RecvData.nScore)
			{
				for (int nCount = 8; nCount >= nCntRank; nCount--)
				{
					//���ʂ�1��������
					g_aRankingData[nCount + 1].nScore = g_aRankingData[nCount].nScore;
				}

				//�󂢂��Ƃ���ɋL�^�^�C��������
				g_aRankingData[nCntRank].nScore = RecvData.nScore;

				//�����L���O�t�@�C���ɏ�������
				WriteFile();

				break;
			}
		}

		//���M����
		aSendBuff[0] = nCntRank + 1;
		pTcpClient->Send(&aSendBuff[0], sizeof(aSendBuff[0]));

		break;

	default:	//�ǂ�ɂ����Ă͂܂�Ȃ������Ƃ�

		break;
	}

	//pTcpClient��j��
	pTcpClient->Release();
}