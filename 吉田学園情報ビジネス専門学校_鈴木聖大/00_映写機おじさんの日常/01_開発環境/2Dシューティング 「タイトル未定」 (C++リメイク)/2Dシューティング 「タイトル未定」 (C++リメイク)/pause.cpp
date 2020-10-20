//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "pause.h"
#include "ui.h"
#include "manager.h"
#include "game.h"
#include "sound.h"
#include "keyboard.h"
#include "controller.h"
#include "fade.h"
#include "player.h"

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CPause::CPause()
{
	memset(m_apMenuUi, 0, sizeof(m_apMenuUi));
	m_pUi = NULL;
	m_pInstructionUi = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nChoiceNum = MENU_CONTINUE;
	m_bStickMove = false;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CPause::~CPause()
{
}

//*****************************************************************************
//�|�[�Y�̐���
//*****************************************************************************
CPause *CPause::Create(void)
{
	//�|�[�Y�̃������m��
	CPause *pPause = new CPause;

	//�|�[�Y�̏���������
	if (pPause != NULL)
	{
		pPause->Init();
	}

	return pPause;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CPause::Init(void)
{
	//SE�̒�~
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_CHARGE);

	//�V�[���̍X�V���~�߂�
	CScene::UpdateStop(true);

	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(OBJTYPE_PAUSE);

	//�|�[�Y���ڂ̐���
	m_apMenuUi[MENU_CONTINUE] = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 200.0f, 0.0f), D3DXVECTOR3(200.0f, 100.0f, 0.0f), CUi::UI_TYPE_CONTINUE);
	m_apMenuUi[MENU_CONTINUE]->SetPauseChoice(true);
	m_apMenuUi[MENU_INSTRUCTION] = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 320.0f, 0.0f), D3DXVECTOR3(260.0f, 110.0f, 0.0f), CUi::UI_TYPE_INSTRUCTION);
	m_apMenuUi[MENU_START_OVER] = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 440.0f, 0.0f), D3DXVECTOR3(250.0f, 100.0f, 0.0f), CUi::UI_TYPE_START_OVER);
	m_apMenuUi[MENU_RETURN_TO_SELECT] = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 560.0f, 0.0f), D3DXVECTOR3(260.0f, 110.0f, 0.0f), CUi::UI_TYPE_RETURN_TO_SELECT);
	
	//�|�[�Y�gUI�̐���
	m_pUi = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(500.0f, 580.0f, 0.0f), CUi::UI_TYPE_PAUSE);

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CPause::Uninit(void)
{
	for (int nCount = 0; nCount < MENU_MAX; nCount++)
	{
		if (m_apMenuUi[nCount] != NULL)
		{
			m_apMenuUi[nCount]->Uninit();
			m_apMenuUi[nCount] = NULL;
		}
	}

	if (m_pUi != NULL)
	{
		m_pUi->Uninit();
		m_pUi = NULL;
	}

	//�������̊J��
	Release();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CPause::Update(void)
{
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	DIJOYSTATE2 Controller = CManager::GetController()->GetController();

	if (m_pInstructionUi == NULL)
	{
		if (m_apMenuUi[m_nChoiceNum] != NULL)
		{
			if (pKeyboard->GetKeyboardTrigger(DIK_S) || pKeyboard->GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0)
			{ //A���������Ƃ� �܂��� �����������Ƃ� �܂��� �X�e�B�b�N�����ɌX�����Ƃ�
				if (m_bStickMove == false)
				{
					//�I�����Ă��鍀�ڂ�����
					m_apMenuUi[m_nChoiceNum]->SetPauseChoice(false);

					//����̍��ڂɈړ�
					m_nChoiceNum++;

					if (m_nChoiceNum == MENU_MAX)
					{ //��ԉ��̍��ڂŉ��Ɉړ������Ƃ�
						//��ԏ�̍��ڂɈړ�����
						m_nChoiceNum = 0;
					}

					//�ړ��������ڂ�_�ł�����
					m_apMenuUi[m_nChoiceNum]->SetPauseChoice(true);
				}

				//�X�e�B�b�N�̈ړ��t���O�𗧂Ă�
				m_bStickMove = true;
			}
			else if (pKeyboard->GetKeyboardTrigger(DIK_W) || pKeyboard->GetKeyboardTrigger(DIK_UP) || Controller.lY < 0)
			{ //W���������Ƃ� �܂��� ����������Ƃ� �܂��� �X�e�B�b�N����ɌX�����Ƃ�
				if (m_bStickMove == false)
				{
					//�I�����Ă��鍀�ڂ�����
					m_apMenuUi[m_nChoiceNum]->SetPauseChoice(false);

					//���̍��ڂɈړ�
					m_nChoiceNum--;

					if (m_nChoiceNum < 0)
					{ //��ԏ�̍��ڂŏ�Ɉړ������Ƃ�
						//��ԉ��̍��ڂɈړ�����
						m_nChoiceNum = MENU_MAX - 1;
					}

					//�ړ��������ڂ�_�ł�����
					m_apMenuUi[m_nChoiceNum]->SetPauseChoice(true);
				}

				//�X�e�B�b�N�̈ړ��t���O�𗧂Ă�
				m_bStickMove = true;
			}
		}

		if (CManager::GetController()->GetController().lX == 0 &&
			CManager::GetController()->GetController().lY == 0)
		{
			//�X�e�B�b�N�̈ړ��t���O��������
			m_bStickMove = false;
		}

		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || CManager::GetController()->GetJoypadTrigger(3))
		{ //Enter�L�[���������Ƃ� �܂��� B�{�^�����������Ƃ�
			for (int nCount = 0; nCount < MENU_MAX; nCount++)
			{
				if (m_apMenuUi[nCount] != NULL)
				{
					if (m_apMenuUi[nCount]->GetPauseChoice() == true)
					{
						switch (nCount)
						{
						case MENU_CONTINUE:

							//�V�[���̍X�V��i�߂�
							CScene::UpdateStop(false);

							//�I������
							Uninit();
							return;

							break;

						case MENU_INSTRUCTION:

							m_pInstructionUi = CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CUi::UI_TYPE_DESCRIPTION);

							break;

						case MENU_START_OVER:

							CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_GAME);

							if (CManager::GetModePointer() != NULL)
							{
								//�X�e�[�W�ԍ��̏�����
								CGame *pGame = dynamic_cast<CGame*>(CManager::GetModePointer());
								pGame->SetStageNum(0);
							}

							if (CManager::GetPlayer() != NULL)
							{
								if (CManager::GetPlayer()->GetPlayerLife() < PLAYER_LIFE)
								{ //���C�t�������Ă����Ƃ�
									//���C�t�̏�����
									CManager::GetPlayer()->SetLife(PLAYER_LIFE);
								}
							}

							break;

						case MENU_RETURN_TO_SELECT:

							CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_SELECT);

							break;
						}

						break;
					}
				}
			}
		}
	}
	else
	{
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_X) || CManager::GetController()->GetJoypadTrigger(CController::BUTTON_X))
		{ //X�L�[ �܂��� X�{�^�����������Ƃ�
			//�I������
			m_pInstructionUi->Uninit();
			m_pInstructionUi = NULL;
		}
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CPause::Draw(void)
{
}