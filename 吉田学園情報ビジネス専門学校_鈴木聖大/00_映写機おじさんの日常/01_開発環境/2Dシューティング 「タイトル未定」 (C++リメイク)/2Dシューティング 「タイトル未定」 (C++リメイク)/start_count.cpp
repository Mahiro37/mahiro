//=============================================================================
//
// �X�^�[�g���̃J�E���g���� [start_count.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "start_count.h"
#include "number.h"
#include "ui.h"
#include "manager.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define COUNT_SPEED		(60)	//�J�E���g�̃X�s�[�h
#define START_COUNT		(3)		//�ŏ��̐���

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CStartCount::CStartCount()
{
	m_pNumber = NULL;
	m_pUi = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nStartNumber = START_COUNT;
	m_nCounter = 0;
	m_nCntStageNum = 0;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CStartCount::~CStartCount()
{
}

//*****************************************************************************
//�X�^�[�g�J�E���g�̐���
//*****************************************************************************
CStartCount *CStartCount::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//�X�^�[�g�J�E���g�̃������m��
	CStartCount *pStartCount = new CStartCount;

	//�X�^�[�g�J�E���g�̏���������
	if (pStartCount != NULL)
	{
		pStartCount->Init(pos, size);
	}

	return pStartCount;
}

HRESULT CStartCount::Init(void)
{
	return S_OK;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CStartCount::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(OBJTYPE_START_OR_FINISH);

	//�V�[���̍X�V���~�߂�
	UpdateStop(true);

	if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
	{
		//�����̐���
		m_pNumber = CNumber::Create(pos, size, 3, CNumber::TYPE_1);

		if (m_pNumber != NULL)
		{
			//�����̐ݒ菈��
			m_pNumber->SetNumber(m_nStartNumber);

			//BGM�̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_COUNT);
		}
	}
	else if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (m_pUi == NULL)
		{
			//�X�e�[�W�ԍ���UI�̐���
			m_pUi = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(450.0f, 200.0f, 0.0f), CUi::UI_TYPE_STAGE_1);

			//BGM�̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_STAGE_NUM);
		}
	}

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CStartCount::Uninit(void)
{
	//�����̏I������
	if (m_pNumber != NULL)
	{
		m_pNumber->Uninit();
		delete m_pNumber;
		m_pNumber = NULL;
	}

	if (m_pUi != NULL)
	{
		//m_pUi->Uninit();
		m_pUi = NULL;
	}

	//�������̊J������
	Release();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CStartCount::Update(void)
{
	//�J�E���g�̃J�E���g��i�߂�
	m_nCounter++;

	switch (CManager::GetMode())
	{
	case CManager::MODE_SCORE_ATTACK:

		if (m_nCounter == COUNT_SPEED)
		{
			m_nCounter = 0;

			m_nStartNumber--;

			if (m_nStartNumber >= 0)
			{
				if (m_nStartNumber != 0)
				{
					//�����̍X�V����
					if (m_pNumber != NULL)
					{
						m_pNumber->SetNumber(m_nStartNumber);
					}
				}
				else
				{
					//�J�E���g�̏I������
					if (m_pNumber != NULL)
					{
						m_pNumber->Uninit();
						delete m_pNumber;
						m_pNumber = NULL;
					}

					//StartUI�̐���
					if (m_pUi == NULL)
					{
						m_pUi = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(400.0f, 200.0f, 0.0f), CUi::UI_TYPE_START);
					}
				}
			}
			else
			{
				//BGM�̍Đ�
				CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM05);

				if (m_pUi != NULL)
				{
					//StartUI�̏I������
					m_pUi->Uninit();
					m_pUi = NULL;
				}

				//�V�[���̍X�V��i�߂�
				UpdateStop(false);

				//�I������
				Uninit();
				return;
			}
		}

		break;

	case CManager::MODE_GAME:

		if (m_pUi != NULL)
		{
			if (m_nStartNumber == 3)
			{
				if (CManager::GetModePointer() != NULL)
				{
					//�e�N�X�`���̍X�V
					CGame *pGame = dynamic_cast<CGame*>(CManager::GetModePointer());
					m_pUi->BindTexture((CUi::UI_TYPE)(CUi::UI_TYPE_STAGE_1 + pGame->GetStageNum()));
				}
			}
		}

		if (m_nCounter == COUNT_SPEED)
		{
			m_nCounter = 0;

			if (m_pNumber == NULL)
			{
				if (m_pUi != NULL)
				{
					if (m_nStartNumber == 3)
					{
						m_nCntStageNum++;

						if (m_nCntStageNum == 2)
						{
							//�X�e�[�W�ԍ���UI�̏I������
							if (m_pUi != NULL)
							{
								m_pUi->Uninit();
								m_pUi = NULL;
							}
						}
						else
						{
							return;
						}
					}
				}
			}
			else
			{
				m_nStartNumber--;
			}

			if (m_nStartNumber >= 0)
			{
				if (m_nStartNumber > 0)
				{
					if (m_pNumber == NULL)
					{
						//�����̐���
						m_pNumber = CNumber::Create(m_pos, m_size, m_nStartNumber, CNumber::TYPE_1);

						//�X�e�[�W�ԍ���UI�̏I������
						if (m_pUi != NULL)
						{
							m_pUi->Uninit();
							m_pUi = NULL;
						}

						//BGM�̍Đ�
						CManager::GetSound()->Play(CSound::SOUND_LABEL_COUNT);

						break;
					}
					else
					{
						if (m_nStartNumber > 0)
						{
							//�����̍X�V����
							m_pNumber->SetNumber(m_nStartNumber);
						}
					}
				}
				else
				{
					//�J�E���g�̏I������
					if (m_pNumber != NULL)
					{
						m_pNumber->Uninit();
						delete m_pNumber;
						m_pNumber = NULL;
					}

					//StartUI�̐���
					if (m_pUi == NULL)
					{
						m_pUi = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(400.0f, 200.0f, 0.0f), CUi::UI_TYPE_START);
					}

					m_nStartNumber--;
				}
			}
			else
			{
				//BGM�̍Đ�
				CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM05);

				if (m_pUi != NULL)
				{
					//StartUI�̏I������
					m_pUi->Uninit();
					m_pUi = NULL;
				}

				//�V�[���̍X�V��i�߂�
				UpdateStop(false);

				//�I������
				Uninit();
				return;
			}
		}

		break;

	default:

		//�J�E���g�̏�����
		m_nCounter = 0;

		break;
	}

	//�����̍X�V����
	if (m_pNumber != NULL)
	{
		m_pNumber->Update();
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CStartCount::Draw(void)
{
	//�����̕`�揈��
	if (m_pNumber != NULL)
	{
		m_pNumber->Draw();
	}
}