//=============================================================================
//
// �����L���O���� [ranking.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include <winsock2.h>
#include "ranking.h"
#include "manager.h"
#include "keyboard.h"
#include "controller.h"
#include "bg.h"
#include "fade.h"
#include "door.h"
#include "score.h"
#include "number.h"
#include "ui.h"
#include "sound.h"

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
CProtocolRanking::RankingData CRanking::m_aRankingData[MAX_RANKING] = {};

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CRanking::CRanking()
{
	memset(m_apScore, 0, sizeof(m_apScore));
	memset(m_apUi, 0, sizeof(m_apUi));
	m_nCntScore = 0;
	m_bUpdateScore = true;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CRanking::~CRanking()
{
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CRanking::Init(void)
{
	//BGM�̍Đ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM03);

	//�����L���O�̎擾
	GetRanking();

	//�w�i�̐���
	CBg::Create();

	//���b�Z�[�WUI�̐���
	CUi::Create(D3DXVECTOR3(300.0f, 100.0f, 0.0f), D3DXVECTOR3(270.0f, 100.0f, 0.0f), CUi::UI_TYPE_RANKING_MESSAGE);

	//�v���C���[�̐���
	CManager::CreatePlayer();

	//�h�A�̐���
	CDoor::Create(D3DXVECTOR3(229.8f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_SELECT);

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CRanking::Uninit(void)
{
	//BGM�̒�~
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM03);

	//�S�Ẵ������̊J������
	CScene::ReleaseAllOtherThanFadeAndPlayer();

	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	{
		if (m_apScore[nCount] != NULL)
		{
			m_apScore[nCount] = NULL;
		}

		if (m_apUi[nCount] != NULL)
		{		
			m_apUi[nCount] = NULL;
		}
	}

	// winsock�I������
	WSACleanup();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CRanking::Update(void)
{
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	DIJOYSTATE2 Controller = CManager::GetController()->GetController();

	D3DXVECTOR3 NumPos;
	D3DXVECTOR3 UiPos;

	if (pKeyboard->GetKeyboardTrigger(DIK_R) || CManager::GetController()->GetJoypadTrigger(1))
	{ //R�L�[ �܂��� Y�{�^�����������Ƃ�
		//�t�F�[�h�̐���
		CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_RANKING);
	}

	if (m_bUpdateScore == true)
	{ //�X�V���K�v�ȂƂ�
		//�L�[�{�[�h�̓��͏���
		if (pKeyboard->GetKeyboardPress(DIK_S) || CManager::GetController()->GetJoypadPress(7))
		{ //S�L�[ �܂��� R2�{�^�����������Ƃ�
			//�X�R�A�𐶐�����J�E���g��i�߂�
			m_nCntScore += 5;
		}
		else
		{
			//�X�R�A�𐶐�����J�E���g��i�߂�
			m_nCntScore++;
		}

		if (m_nCntScore >= 120)
		{ //120�t���[���o�����Ƃ�
			for (int nCount = MAX_RANKING - 1; nCount > 2; nCount--)
			{
				if (m_apScore[nCount] == NULL)
				{
					//�X�R�A�̐���(10�ʂ���)
					m_apScore[nCount] = CScore::Create(D3DXVECTOR3(900.0f, -50.0f, 0.0f), D3DXVECTOR3(80.0f, 100.0f, 0.0f),40.0f, m_aRankingData[nCount].nScore, CNumber::TYPE_1);

					for (int nCntUi = 0; nCntUi < 7; nCntUi++)
					{
						if (m_apUi[nCntUi] == NULL)
						{
							//����UI�̐���
							m_apUi[nCntUi] = CUi::Create(D3DXVECTOR3(600.0f, -50.0f, 0.0f), D3DXVECTOR3(140.0f, 100.0f, 0.0f), (CUi::UI_TYPE)(CUi::UI_TYPE_RANKING_10 - nCntUi));

							break;
						}
					}

					//�J�E���g�̏�����
					m_nCntScore = 0;

					break;
				}
			}
		}

		if (m_apScore[3] != NULL)
		{ //4�ʂ���������Ă���Ƃ�
			if (m_nCntScore >= 210)
			{ //240�t���[���o�����Ƃ�
				for (int nCount = 2; nCount >= 0; nCount--)
				{
					if (m_apScore[nCount] == NULL)
					{
						//�X�R�A�̐���(3�ʂ���)
						m_apScore[nCount] = CScore::Create(D3DXVECTOR3(900.0f, -50.0f, 0.0f), D3DXVECTOR3(80.0f, 100.0f, 0.0f), 40.0f, m_aRankingData[nCount].nScore, CNumber::TYPE_1);

						for (int nCntUi = 7; nCntUi < 10; nCntUi++)
						{
							if (m_apUi[nCntUi] == NULL)
							{
								//����UI�̐���
								m_apUi[nCntUi] = CUi::Create(D3DXVECTOR3(600.0f, -50.0f, 0.0f), D3DXVECTOR3(140.0f, 100.0f, 0.0f), (CUi::UI_TYPE)(CUi::UI_TYPE_RANKING_10 - nCntUi));
							
								break;
							}
						}

						//�J�E���g�̏�����
						m_nCntScore = 0;

						break;
					}
				}
			}
		}

		for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
		{
			if (m_apScore[nCntRank] != NULL)
			{
				for (int nCntNum = 0; nCntNum < SCORE_NUM; nCntNum++)
				{
					if (m_apScore[nCntRank]->GetNumber(nCntNum) != NULL)
					{
						//�X�R�A�̈ʒu�̎擾
						NumPos = m_apScore[nCntRank]->GetNumber(nCntNum)->GetPos();

						if (nCntRank == 1)
						{ //2�ʂ̈ʒu��
							if (NumPos.y >= 360.0f)
							{ //�����܂ŗ����Ƃ�
								//�X�R�A�̍X�V���~�߂�
								m_bUpdateScore = false;
								return;
							}
						}

						//�L�[�{�[�h�̓��͏���
						if (pKeyboard->GetKeyboardPress(DIK_S) || CManager::GetController()->GetJoypadPress(7))
						{ //S�L�[ �܂��� R2�{�^�����������Ƃ�
							//�X�R�A�̈ʒu�̍X�V
							NumPos.y += 5.0f;
						}
						else
						{
							//�X�R�A�̈ʒu�̍X�V
							NumPos.y += 1.0f;
						}
						m_apScore[nCntRank]->GetNumber(nCntNum)->SetPos(NumPos);

						if (NumPos.y - 50.0f > SCREEN_HEIGHT)
						{ //�X�R�A����ʊO�ɏo���Ƃ�
							//�X�R�A�̏I������
							m_apScore[nCntRank]->Uninit();
						}
						else if (m_apScore[nCntRank] != NULL)
						{
							//�X�R�A�̍X�V����
							m_apScore[nCntRank]->Update();
						}
					}
				}
			}

			if (m_apUi[nCntRank] != NULL)
			{
				//����UI�̈ʒu�̎擾
				UiPos = m_apUi[nCntRank]->GetPos();

				//�L�[�{�[�h�̓��͏���
				if (pKeyboard->GetKeyboardPress(DIK_S) || CManager::GetController()->GetJoypadPress(7))
				{ //S�L�[ �܂��� R2�{�^�����������Ƃ�
					//����UI�̈ʒu�̍X�V
					UiPos.y += 5.0f;
				}
				else
				{
					//����UI�̈ʒu�̍X�V
					UiPos.y += 1.0f;
				}

				m_apUi[nCntRank]->SetPos(UiPos);

				if (UiPos.y - 50.0f > SCREEN_HEIGHT)
				{ //����UI����ʊO�ɏo���Ƃ�
					//����UI�̏I������
					m_apUi[nCntRank]->Uninit();
					m_apUi[nCntRank] = NULL;
				}

				if (m_apUi[nCntRank] != NULL)
				{
					//����UI�̍X�V����
					m_apUi[nCntRank]->Update();
				}
			}
		}
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CRanking::Draw(void)
{
}

//*****************************************************************************
// �����L���O�̐ݒ�
//*****************************************************************************
int CRanking::SetRanking(int nScore)
{
	WSADATA wsaData;
	char aDestlPAddr[BUFFER_NUM] = "127.0.0.1";

	// winsock����������
	WSAStartup(WINSOCK_VERSION, &wsaData);

	//�������̊m��
	CTcpClient *m_pTcpClient = NULL;
	m_pTcpClient = m_pTcpClient->Create(IP_ADDRESS, SERVER_PORT_NUM);

	if (m_pTcpClient == NULL)
	{
		// winsock�I������
		WSACleanup();
	}

	char aSendBuf[BUFFER_NUM];

	memset(aSendBuf, 0, sizeof(aSendBuf));

	//1�Ԗڂ̔z��ɂQ������
	aSendBuf[0] = CProtocolRanking::COMMAND_TYPE_SET_RANKING;

	//�G���f�B�A���ϊ�
	nScore = htonl(nScore);

	//2�Ԗڂ̔z�񂩂�4�o�C�g����Time�f�[�^���i�[(Time(int�^)��4�o�C�g������)
	memcpy(&aSendBuf[sizeof(aSendBuf[0])], &nScore, sizeof(nScore));

	//�����L���O�ݒ胊�N�G�X�g�̑��M
	m_pTcpClient->Send(aSendBuf, sizeof(aSendBuf[0]) + sizeof(nScore));

	char aRecvBuf[BUFFER_NUM];
	memset(aRecvBuf, 0, sizeof(aRecvBuf));

	//�����L���O���ʂ���M
	m_pTcpClient->Recv(aRecvBuf, sizeof(aRecvBuf[0]));

	//�N���C�A���g�̏I������
	m_pTcpClient->Release();

	return aRecvBuf[0];
}

//*****************************************************************************
// �����L���O�̎擾
//*****************************************************************************
void CRanking::GetRanking(void)
{
	WSADATA wsaData;
	char aDestlPAddr[BUFFER_NUM] = "127.0.0.1";

	//winsock����������
	WSAStartup(WINSOCK_VERSION, &wsaData);

	//�������̊m��
	CTcpClient *m_pTcpClient = NULL;
	m_pTcpClient = m_pTcpClient->Create(IP_ADDRESS, SERVER_PORT_NUM);

	if (m_pTcpClient == NULL)
	{
		//winsock�I������
		WSACleanup();
	}

	char aSendBuf[BUFFER_NUM];
	memset(aSendBuf, 0, sizeof(aSendBuf));
	aSendBuf[0] = CProtocolRanking::COMMAND_TYPE_GET_RANKING;

	//�����L���O�ꗗ�擾���N�G�X�g�̑��M
	m_pTcpClient->Send(&aSendBuf[0], sizeof(aSendBuf[0]));	//�ŏ��̂P�o�C�g�𑗂肽������P�o�C�g���w��

	//�����L���O�ꗗ�擾���X�|���X��M
	char aRecvBuf[BUFFER_NUM];
	memset(aRecvBuf, 0, sizeof(aRecvBuf));

	//��M����
	m_pTcpClient->Recv(aRecvBuf, sizeof(aRecvBuf));

	int score = 0;

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		memcpy(&score, &aRecvBuf[nCntRank * sizeof(int)], sizeof(int));
		score = ntohl(score);
		m_aRankingData[nCntRank].nScore = score;
	}

	//�N���C�A���g�̏I������
	m_pTcpClient->Release();
}