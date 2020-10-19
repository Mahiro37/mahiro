//=============================================================================
//
// UI���� [ui.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "ui.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define	LOGO_MOVE_AMOUNT	(0.5f)	//�h�A�p���SUI�̈ړ���

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CUi::m_apTexture[UI_TYPE_MAX] = {};

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CUi::CUi()
{
	m_type = UI_TYPE_NONE;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nCntColor = 0;
	m_nCntUi = 0;
	m_bLogoMove = false;
	m_bTransparence = false;
	m_bPauseChoice = false;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CUi::~CUi()
{
}

//*****************************************************************************
//���[�h����
//*****************************************************************************
HRESULT CUi::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/timer.png", &m_apTexture[UI_TYPE_TIMER]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/zanki.png", &m_apTexture[UI_TYPE_ZANKI]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/film.png", &m_apTexture[UI_TYPE_FILM]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/score.png", &m_apTexture[UI_TYPE_SCORE]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/playerSpeedUpItem.png", &m_apTexture[UI_TYPE_PLAYER_SPEED_UP]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/bulletSpeedUpItem.png", &m_apTexture[UI_TYPE_BULLET_SPEED_UP]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/barrierItem.png", &m_apTexture[UI_TYPE_BARRIER]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/ranking01.png", &m_apTexture[UI_TYPE_RANKING_1]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/ranking02.png", &m_apTexture[UI_TYPE_RANKING_2]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/ranking03.png", &m_apTexture[UI_TYPE_RANKING_3]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/ranking04.png", &m_apTexture[UI_TYPE_RANKING_4]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/ranking05.png", &m_apTexture[UI_TYPE_RANKING_5]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/ranking06.png", &m_apTexture[UI_TYPE_RANKING_6]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/ranking07.png", &m_apTexture[UI_TYPE_RANKING_7]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/ranking08.png", &m_apTexture[UI_TYPE_RANKING_8]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/ranking09.png", &m_apTexture[UI_TYPE_RANKING_9]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/ranking10.png", &m_apTexture[UI_TYPE_RANKING_10]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/start.png", &m_apTexture[UI_TYPE_START]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/finish.png", &m_apTexture[UI_TYPE_FINISH]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/game_over.png", &m_apTexture[UI_TYPE_GAME_OVER]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/game_clear.png", &m_apTexture[UI_TYPE_GAME_CLEAR]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/stage1.png", &m_apTexture[UI_TYPE_STAGE_1]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/stage2.png", &m_apTexture[UI_TYPE_STAGE_2]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/stage3.png", &m_apTexture[UI_TYPE_STAGE_3]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/score_screen.png", &m_apTexture[UI_TYPE_GAME_SCORE_SCREEN]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/rank_in.png", &m_apTexture[UI_TYPE_RANK_IN]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/select.png", &m_apTexture[UI_TYPE_SELECT]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/select_title.png", &m_apTexture[UI_TYPE_SELECT_TITLE]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/select_stage.png", &m_apTexture[UI_TYPE_SELECT_GAME]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/select_score_attack.png", &m_apTexture[UI_TYPE_SELECT_SCORE_ATTACK]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/select_tutorial.png", &m_apTexture[UI_TYPE_SELECT_TUTORIAL]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/select_ranking.png", &m_apTexture[UI_TYPE_SELECT_RANKING]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/select_select.png", &m_apTexture[UI_TYPE_SELECT_SELECT]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/ball_create.png", &m_apTexture[UI_TYPE_BALL_CREATE]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/tutorial_stage.png", &m_apTexture[UI_TYPE_TUTORIAL_GAME]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/tutorial_score_attack.png", &m_apTexture[UI_TYPE_TUTORIAL_SCORE_ATTACK]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/fukidashi.png", &m_apTexture[UI_TYPE_FUKIDASHI]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/ranking_message.png", &m_apTexture[UI_TYPE_RANKING_MESSAGE]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/description.png", &m_apTexture[UI_TYPE_DESCRIPTION]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/title.png", &m_apTexture[UI_TYPE_TITLE]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/continue.png", &m_apTexture[UI_TYPE_CONTINUE]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/instructions.png", &m_apTexture[UI_TYPE_INSTRUCTION]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/start_over.png", &m_apTexture[UI_TYPE_START_OVER]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/return_to_select.png", &m_apTexture[UI_TYPE_RETURN_TO_SELECT]);
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/pause.png", &m_apTexture[UI_TYPE_PAUSE]);

	return S_OK;
}

//*****************************************************************************
//�A�����[�h����
//*****************************************************************************
void CUi::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCount = 0; nCount < UI_TYPE_MAX; nCount++)
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
CUi *CUi::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI_TYPE type)
{
	//UI�̃������m��
	CUi *pUi = new CUi;

	if (pUi != NULL)
	{
		//UI�̏���������
		pUi->Init(pos, size, type);
	}

	return pUi;
}

HRESULT CUi::Init(void)
{
	return S_OK;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CUi::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI_TYPE type)
{
	//�^�C�v�̐ݒ�
	m_type = type;

	switch (type)
	{
	case UI_TYPE_RANK_IN:

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_UI_FRONT);

		//�J���[�̐ݒ�
		SetColor(m_color);

		break;

	case UI_TYPE_STAGE_1:
	case UI_TYPE_STAGE_2:
	case UI_TYPE_STAGE_3:

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_START_OR_FINISH);

		m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//�J���[�̐ݒ�
		SetColor(m_color);

		break;

	case UI_TYPE_PLAYER_SPEED_UP:
	case UI_TYPE_BULLET_SPEED_UP:
	case UI_TYPE_BARRIER:

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_UI_ITEM);

		break;

	case UI_TYPE_RANKING_1:
	case UI_TYPE_RANKING_2:
	case UI_TYPE_RANKING_3:
	case UI_TYPE_RANKING_4:
	case UI_TYPE_RANKING_5:
	case UI_TYPE_RANKING_6:
	case UI_TYPE_RANKING_7:
	case UI_TYPE_RANKING_8:
	case UI_TYPE_RANKING_9:
	case UI_TYPE_RANKING_10:
	case UI_TYPE_GAME_SCORE_SCREEN:
	case UI_TYPE_SELECT_TITLE:
	case UI_TYPE_SELECT_GAME:
	case UI_TYPE_SELECT_SCORE_ATTACK:
	case UI_TYPE_SELECT_TUTORIAL:
	case UI_TYPE_SELECT_RANKING:
	case UI_TYPE_SELECT_SELECT:
	case UI_TYPE_BALL_CREATE:

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_UI_BACK);

		break;

	case UI_TYPE_DESCRIPTION:

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_DESCRIPTION);

		//SE�̍Đ�
		CManager::GetSound()->Play(CSound::SOUND_LABEL_PAPER_OPEN);

		break;

	case UI_TYPE_CONTINUE:
	case UI_TYPE_INSTRUCTION:
	case UI_TYPE_START_OVER:
	case UI_TYPE_RETURN_TO_SELECT:

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_PAUSE);

		break;

	default:

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_UI_FRONT);

		break;
	}

	//�^�C�v�̐ݒ�
	CScene2D::BindTexture(m_apTexture[type]);

	//�ʒu�̐ݒ�
	SetPos(pos);

	//�T�C�Y�̐ݒ�
	SetSize(size);

	//�V�[��2D�̏�����
	CScene2D::Init();

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CUi::Uninit(void)
{
	//�V�[��2D�̏I������
	CScene2D::Uninit();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CUi::Update(void)
{
	if (m_type == UI_TYPE_RANK_IN ||
		m_type == UI_TYPE_FUKIDASHI)
	{
		if (m_color.a < 1.0f)
		{ //�����x��1.0f�ȉ��̂Ƃ�
			//�J���[���X�V����J�E���g��i�߂�
			m_nCntColor++;

			if (m_nCntColor == 1)
			{ //1�t���[���o�����Ƃ�
				//�J�E���g�̏�����
				m_nCntColor = 0;

				//�����x�̉��Z
				m_color.a += 0.1f;

				//�J���[�̍X�V
				SetColor(m_color);
			}
		}
		else
		{
			//�J���[���X�V����J�E���g��i�߂�
			m_nCntColor++;

			if (m_nCntColor == 15)
			{ //15�t���[���o�����Ƃ�
				//�J���[�̍X�V
				m_color = D3DXCOLOR(1.0f, 1.0f, 0.8f, 1.0f);
			}
			else if (m_nCntColor == 30)
			{ //30�t���[���o�����Ƃ�
				//�J�E���g�̏�����
				m_nCntColor = 0;

				//�J���[�̍X�V
				m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			//�J���[�̐ݒ�
			SetColor(m_color);
		}
	}
	else if (m_type == UI_TYPE_STAGE_1 ||
			 m_type == UI_TYPE_STAGE_2 ||
			 m_type == UI_TYPE_STAGE_3)
	{
		if (m_color.a < 1.0f)
		{ //�����x��1.0f�ȉ��̂Ƃ�
			//�J���[���X�V����J�E���g��i�߂�
			m_nCntColor++;

			if (m_nCntColor == 1)
			{ //1�t���[���o�����Ƃ�
				//�J�E���g�̏�����
				m_nCntColor = 0;

				//�����x�̉��Z
				m_color.a += 0.1f;

				//�J���[�̍X�V
				SetColor(m_color);
			}
		}
	}
	else if (m_type == UI_TYPE_SELECT_TITLE ||
			 m_type == UI_TYPE_SELECT_GAME ||
			 m_type == UI_TYPE_SELECT_SCORE_ATTACK ||
			 m_type == UI_TYPE_SELECT_TUTORIAL ||
			 m_type == UI_TYPE_SELECT_RANKING ||
			 m_type == UI_TYPE_SELECT_SELECT)
	{
		D3DXVECTOR3 pos = GetPos();

		if (m_bLogoMove == false)
		{
			m_move.y -= 0.05f;

			if (m_move.y <= -LOGO_MOVE_AMOUNT)
			{
				m_bLogoMove = true;
			}
		}
		else
		{
			m_move.y += 0.05f;

			if (m_move.y >= LOGO_MOVE_AMOUNT)
			{
				m_bLogoMove = false;
			}
		}

		pos += m_move;

		//�ʒu�̍X�V
		SetPos(pos);
	}
	else if (m_type == UI_TYPE_SELECT)
	{
		D3DXVECTOR3 pos = GetPos();

		m_nCntUi++;

		if (m_nCntUi == 8)
		{
			pos.y += 0.5f;
		}
		else if (m_nCntUi == 16)
		{
			m_nCntUi = 0;

			pos.y -= 0.5f;
		}

		//�ʒu�̍X�V
		SetPos(pos);
	}
	else if (m_type == UI_TYPE_TUTORIAL_GAME ||
			 m_type == UI_TYPE_TUTORIAL_SCORE_ATTACK ||
			 m_type == UI_TYPE_RANKING_MESSAGE)
	{
		//�J���[���X�V����J�E���g��i�߂�
		m_nCntColor++;

		if (m_nCntColor == 3)
		{ //3�t���[���o�����Ƃ�
			//�J�E���g�̏�����
			m_nCntColor = 0;

			if (m_bTransparence == false)
			{
				//�����x�̌��Z
				m_color.a -= 0.1f;

				if (m_color.a <= 0.0f)
				{
					m_bTransparence = true;
				}
			}
			else
			{
				//�����x�̉��Z
				m_color.a += 0.1f;

				if (m_color.a >= 1.0f)
				{
					m_bTransparence = false;
				}
			}

			//�J���[�̍X�V
			SetColor(m_color);
		}
	}
	else if (m_type == UI_TYPE_CONTINUE ||
			 m_type == UI_TYPE_INSTRUCTION ||
			 m_type == UI_TYPE_START_OVER ||
			 m_type == UI_TYPE_RETURN_TO_SELECT)
	{
		if (m_bPauseChoice == true)
		{
			//�J���[���X�V����J�E���g��i�߂�
			m_nCntColor++;

			if (m_nCntColor == 3)
			{ //3�t���[���o�����Ƃ�
				//�J�E���g�̏�����
				m_nCntColor = 0;

				if (m_bTransparence == false)
				{
					//�����x�̌��Z
					m_color.a -= 0.1f;

					if (m_color.a <= 0.0f)
					{
						m_bTransparence = true;
					}
				}
				else
				{
					//�����x�̉��Z
					m_color.a += 0.1f;

					if (m_color.a >= 1.0f)
					{
						m_bTransparence = false;
					}
				}

				//�J���[�̍X�V
				SetColor(m_color);
			}
		}
		else
		{
			//�����x�̏�����
			m_color.a = 1.0f;

			//�J���[�̍X�V
			SetColor(m_color);
		}
	}
	else if (m_type == UI_TYPE_TITLE ||
			 m_type == UI_TYPE_GAME_CLEAR)
	{
		D3DXVECTOR3 pos = GetPos();

		m_nCntUi++;

		if (m_nCntUi == 7)
		{
			pos.y += 0.5f;
		}
		else if (m_nCntUi == 14)
		{
			m_nCntUi = 0;

			pos.y -= 0.5f;
		}

		//�ʒu�̍X�V
		SetPos(pos);
	}

	//�V�[��2D�̍X�V����
	CScene2D::Update();
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CUi::Draw(void)
{
	//�V�[��2D�̕`�揈��
	CScene2D::Draw();
}

//*****************************************************************************
//�|�[�Y�̑I����Ԃ̐ݒ�
//*****************************************************************************
void CUi::SetPauseChoice(bool bPauseChoice)
{
	m_bPauseChoice = bPauseChoice;
}

//*****************************************************************************
//�e�N�X�`���̐ݒ�
//*****************************************************************************
void CUi::BindTexture(UI_TYPE type)
{
	CScene2D::BindTexture(m_apTexture[type]);
}

//*****************************************************************************
//�|�[�Y�̑I����Ԃ̎擾
//*****************************************************************************
bool CUi::GetPauseChoice(void)
{
	return m_bPauseChoice;
}

//*****************************************************************************
//UI�^�C�v�̎擾
//*****************************************************************************
CUi::UI_TYPE CUi::GetUiType(void)
{
	return m_type;
}