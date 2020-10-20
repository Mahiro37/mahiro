//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "fade.h"
#include "renderer.h"
#include "manager.h"
#include "score_attack.h"
#include "game.h"
#include "tutorial_score_attack.h"
#include "ScoreScreen.h"
#include "start_count.h"
#include "time.h"
#include "player.h"
#include "life.h"
#include "score.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CFade::m_apTexture[FADE_MAX] = {};
CFade *CFade::m_pFade = NULL;
CStartCount *CFade::m_pStartCount = NULL;
int CFade::m_nStageNum = 0;
CManager::MODE CFade::m_mode = CManager::MODE_NONE;

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CFade::CFade()
{
	m_nCntFade = 0;
	m_nCntTexture = 0;
	m_bWrapBack = false;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CFade::~CFade()
{
}

//*****************************************************************************
//���[�h����
//*****************************************************************************
HRESULT CFade::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade00.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade01.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade02.png", &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade03.png", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade04.png", &m_apTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade05.png", &m_apTexture[5]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade06.png", &m_apTexture[6]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade07.png", &m_apTexture[7]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade08.png", &m_apTexture[8]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade09.png", &m_apTexture[9]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade10.png", &m_apTexture[10]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade11.png", &m_apTexture[11]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade12.png", &m_apTexture[12]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade13.png", &m_apTexture[13]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade14.png", &m_apTexture[14]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade15.png", &m_apTexture[15]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade16.png", &m_apTexture[16]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade17.png", &m_apTexture[17]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade18.png", &m_apTexture[18]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade19.png", &m_apTexture[19]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade20.png", &m_apTexture[20]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade21.png", &m_apTexture[21]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade22.png", &m_apTexture[22]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade23.png", &m_apTexture[23]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade24.png", &m_apTexture[24]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade25.png", &m_apTexture[25]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade26.png", &m_apTexture[26]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade27.png", &m_apTexture[27]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade28.png", &m_apTexture[28]);
	D3DXCreateTextureFromFile(pDevice, "Textures/fade/fade29.png", &m_apTexture[29]);

	return S_OK;
}

//*****************************************************************************
//�A�����[�h����
//*****************************************************************************
void CFade::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCount = 0; nCount < FADE_MAX; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//�����̐���
//*****************************************************************************
CFade *CFade::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE mode)
{
	//�t�F�[�h�̃������m��
	if (m_pFade == NULL)
	{
		m_pFade = new CFade;

		if (m_pFade != NULL)
		{
			//�t�F�[�h�̏���������
			m_pFade->Init(pos, size, mode);
		}
	}

	return m_pFade;
}

HRESULT CFade::Init(void)
{
	return S_OK;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CFade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE mode)
{
	//BGM�̍Đ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_CLOSE);

	//���[�h�̐ݒ�
	m_mode = mode;

	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(OBJTYPE_FADE);

	//�e�N�X�`���̐ݒ�
	BindTexture(m_apTexture[0]);

	//�ʒu�̐ݒ�
	SetPos(pos);

	//�T�C�Y�̐ݒ�
	SetSize(size);

	//�V�[��2D�̏�����
	CScene2D::Init();

	//�܂�Ԃ�����̏�����
	m_bWrapBack = false;

	//�J�E���g�̏�����
	m_nCntFade = 0;
	m_nCntTexture = 0;

	//�V�[���̍X�V���~�߂�
	UpdateStop(true);

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CFade::Uninit(void)
{
	//�V�[��2D�̏I������
	CScene2D::Uninit();

	if (m_pFade != NULL)
	{
		m_pFade = NULL;
	}

	if (m_pStartCount != NULL)
	{
		m_pStartCount = NULL;
	}
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CFade::Update(void)
{
	if (m_bWrapBack == false)
	{ //�t�F�[�h���܂�Ԃ��Ă��Ȃ��Ƃ�
		//�t���[���J�E���g��i�߂�
		m_nCntFade++;

		if (m_nCntFade == 2)
		{ //2�t���[���o������
			//�t���[���J�E���g������������
			m_nCntFade = 0;

			//�e�N�X�`���J�E���g��i�߂�
			m_nCntTexture++;

			if (m_nCntTexture < FADE_MAX)
			{ //�e�N�X�`���J�E���g��FADE_MAX��菬�����Ƃ�
				//�e�N�X�`���̐ݒ�
				BindTexture(m_apTexture[m_nCntTexture]);
			}
			else
			{ //�e�N�X�`���J�E���g��FADE_MAX�ȏ�̂Ƃ�
				if (CManager::GetMode() == CManager::MODE_GAME ||
					CManager::GetMode() == CManager::MODE_SCORE_ATTACK ||
					CManager::GetMode() == CManager::MODE_TUTORIAL_GAME ||
					CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK ||
					CManager::GetMode() == CManager::MODE_SCORE_SCREEN ||
					CManager::GetMode() == CManager::MODE_GAME_CREAR)
				{
					if (m_mode == CManager::MODE_SELECT)
					{
						if (CManager::GetPlayer() != NULL)
						{ //���C�t�̃��Z�b�g
							if (CManager::GetPlayer()->GetPlayerLife() < PLAYER_LIFE)
							{ //���C�t�������Ă����Ƃ�
								//���C�t�̏�����
								CManager::GetPlayer()->SetLife(PLAYER_LIFE);
							}
						}

						if (CManager::GetMode() == CManager::MODE_GAME)
						{
							if (CManager::GetModePointer() != NULL)
							{
								//�X�e�[�W�ԍ��̏�����
								CGame *pGame = dynamic_cast<CGame*>(CManager::GetModePointer());
								pGame->SetStageNum(0);
							}
						}
					}

					if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK ||
						CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK ||
						CManager::GetMode() == CManager::MODE_SCORE_SCREEN)
					{
						if (m_mode == CManager::MODE_SCORE_SCREEN)
						{ //�X�R�A�X�N���[���ɑJ�ڂ���Ƃ�
							if (CScoreAttack::GetScore() != NULL && CManager::GetTime() != NULL)
							{
								//�X�R�A�̎擾
								int nScore = CScoreAttack::GetScore()->GetScore();

								//�����^�C���̎擾
								int nTotalTime = CManager::GetTime()->GetTotalTime();
								CManager::GetTime()->ResetTotalTime();

								//�X�R�A�X�N���[���ɃX�R�A��ۑ�
								CScoreScreen::SaveScore(nScore);

								//�X�R�A�X�N���[���ɑ����^�C����ۑ�
								CScoreScreen::SaveTotalTime(nTotalTime);

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//�X�R�A�̏�����
									CScoreAttack::GetScore()->SetScore(0);
								}

								//�X�R�A�X�N���[���Ɏc�@��ۑ�
								CScoreScreen::SavePlayerLife(CManager::GetPlayer()->GetPlayerLife());
							}
						}

						if (m_mode == CManager::MODE_SELECT ||
							m_mode == CManager::MODE_TUTORIAL_SCORE_ATTACK)
						{
							if (CManager::GetTime() != NULL)
							{ //���Ԃ̃��Z�b�g
								if (CManager::GetTime()->GetSaveTime() < TIME_START_NUM)
								{ //�^�C���������Ă����Ƃ�
								  //�^�C���̏�����
									CManager::GetTime()->SaveTime(TIME_START_NUM);
								}
							}

							if (CScoreAttack::GetScore() != NULL)
							{
								//�X�R�A�̏�����
								CScoreAttack::GetScore()->SetScore(0);
							}
						}
					}
				}

				//�����true�ɂ���
				m_bWrapBack = true;

				//���[�h�̐ݒ�
				GetManager()->SetMode(m_mode);

				//BGM�̍Đ�
				CManager::GetSound()->Play(CSound::SOUND_LABEL_OPEN);
			}
		}
	}
	else if (m_bWrapBack == true)
	{ //�t�F�[�h���܂�Ԃ��Ă���Ƃ�
		//�t���[���J�E���g��i�߂�
		m_nCntFade++;

		if (m_nCntFade == 2)
		{ //2�t���[���o������
			//�t���[���J�E���g������������
			m_nCntFade = 0;

			//�e�N�X�`���J�E���g��ނ�
			m_nCntTexture--;
			
			if (m_nCntTexture >= 0)
			{ //�e�N�X�`���J�E���g��0�ȏ�̂Ƃ�
				//�e�N�X�`���̐ݒ�
				BindTexture(m_apTexture[m_nCntTexture]);
			}
			else
			{ //�e�N�X�`���J�E���g��0��菬�����Ƃ�
				//�J�E���g�̏�����
				m_nCntTexture = 0;

				//����̏�����
				m_bWrapBack = false;

				if (m_mode == CManager::MODE_GAME || m_mode == CManager::MODE_SCORE_ATTACK)
				{ //�X�e�[�W���[�h�܂��̓X�R�A�A�^�b�N���[�h�̂Ƃ�
					if (m_pStartCount == NULL)
					{
						//�X�^�[�g�J�E���g�̐���
						m_pStartCount = CStartCount::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(120.0f, 150.0f, 0.0f));
					}

					if (m_mode == CManager::MODE_GAME)
					{ //�X�e�[�W���[�h�̂Ƃ�
						if (CManager::GetModePointer() != NULL)
						{
							//�X�e�[�W�ԍ��̏�����
							CGame *pGame = dynamic_cast<CGame*>(CManager::GetModePointer());
							pGame->SetStageNum(pGame->GetStageNum());
						}
					}
				}
				else
				{
					//�V�[���̍X�V��i�߂�
					UpdateStop(false);
				}

				//�I������
				Uninit();
				return;
			}
		}
	}

	//�V�[��2D�̍X�V����
	CScene2D::Update();
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CFade::Draw(void)
{
	//�V�[��2D�̕`�揈��
	CScene2D::Draw();
}

//*****************************************************************************
//�X�e�[�W�ԍ��̐ݒ�
//*****************************************************************************
void CFade::SetStageNum(int nStageNum)
{
	m_nStageNum = nStageNum;
}

//*****************************************************************************
//�t�F�[�h���܂�Ԃ��Ă��邩�ǂ���
//*****************************************************************************
bool CFade::GetWrapBack(void)
{
	return m_bWrapBack;
}