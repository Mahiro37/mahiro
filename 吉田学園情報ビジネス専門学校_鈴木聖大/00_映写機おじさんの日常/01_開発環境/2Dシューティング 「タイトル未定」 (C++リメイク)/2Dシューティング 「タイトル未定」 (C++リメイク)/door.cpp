//=============================================================================
//
// �h�A���� [door.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "door.h"
#include "renderer.h"
#include "keyboard.h"
#include "controller.h"
#include "player.h"
#include "fade.h"
#include "ui.h"
#include "sound.h"

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CDoor::m_pTexture = NULL;

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CDoor::CDoor()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mode = CManager::MODE_NONE;
	m_nCntLit = 0;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CDoor::~CDoor()
{
}

//*****************************************************************************
//���[�h����
//*****************************************************************************
HRESULT CDoor::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/door.png", &m_pTexture);

	return S_OK;
}

//*****************************************************************************
//�A�����[�h����
//*****************************************************************************
void CDoor::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//*****************************************************************************
//�����̐���
//*****************************************************************************
CDoor *CDoor::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE mode)
{
	//�����̃������m��
	CDoor *pDoor = new CDoor;

	if (pDoor != NULL)
	{
		//�����̏���������
		pDoor->Init(pos, size, mode);
	}

	return pDoor;
}

HRESULT CDoor::Init(void)
{
	return S_OK;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CDoor::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE mode)
{
	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(OBJTYPE_DOOR);

	//�^�C�v�̐ݒ�
	BindTexture(m_pTexture);

	//���[�h�̐ݒ�
	m_mode = mode;

	//�ʒu�̐ݒ�
	m_pos = pos;
	SetPos(pos);

	//�T�C�Y�̐ݒ�
	SetSize(size);

	//�V�[��2D�̏�����
	CScene2D::Init();

	//�h�A�p���SUI�̐���
	switch (mode)
	{
	case CManager::MODE_TITLE:

		CUi::Create(D3DXVECTOR3(pos.x, pos.y - 160.0f, pos.z), D3DXVECTOR3(150.0f, 70.0f, 0.0f), CUi::UI_TYPE_SELECT_TITLE);

		break;

	case CManager::MODE_SELECT:

		CUi::Create(D3DXVECTOR3(pos.x, pos.y - 160.0f, pos.z), D3DXVECTOR3(150.0f, 70.0f, 0.0f), CUi::UI_TYPE_SELECT_SELECT);

		break;

	case CManager::MODE_GAME:

		CUi::Create(D3DXVECTOR3(pos.x, pos.y - 160.0f, pos.z), D3DXVECTOR3(150.0f, 70.0f, 0.0f), CUi::UI_TYPE_SELECT_GAME);

		break;

	case CManager::MODE_SCORE_ATTACK:

		CUi::Create(D3DXVECTOR3(pos.x, pos.y - 165.0f, pos.z), D3DXVECTOR3(160.0f, 80.0f, 0.0f), CUi::UI_TYPE_SELECT_SCORE_ATTACK);

		break;

	case CManager::MODE_TUTORIAL_GAME:

		CUi::Create(D3DXVECTOR3(pos.x, pos.y - 165.0f, pos.z), D3DXVECTOR3(160.0f, 80.0f, 0.0f), CUi::UI_TYPE_SELECT_TUTORIAL);

		break;

	case CManager::MODE_RANKING:

		CUi::Create(D3DXVECTOR3(pos.x, pos.y - 160.0f, pos.z), D3DXVECTOR3(150.0f, 70.0f, 0.0f), CUi::UI_TYPE_SELECT_RANKING);

		break;

	default:

		break;
	}

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CDoor::Uninit(void)
{
	//�V�[��2D�̏I������
	CScene2D::Uninit();

	m_mode = CManager::MODE_NONE;
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CDoor::Update(void)
{
	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		//�I�u�W�F�N�g�̎擾
		CScene *pScene = GetScene(nCntScene);

		//�_�C�i�~�b�N�L���X�g
		CPlayer *pPlayer = dynamic_cast<CPlayer*>(pScene);

		if (pPlayer != NULL)
		{
			D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

			if (PlayerPos.x < m_pos.x + (DOOR_SIZE.x / 2) &&
				PlayerPos.x > m_pos.x - (DOOR_SIZE.x / 2) &&
				PlayerPos.y < m_pos.y + (DOOR_SIZE.y / 2) &&
				PlayerPos.y > m_pos.y - (DOOR_SIZE.y / 2))
			{
				//�_���J�E���g��i�߂�
				m_nCntLit++;

				if (m_nCntLit < 30)
				{ //30�t���[����菬�����Ƃ�
					//�J���[�̍X�V
					SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
				}
				else if (m_nCntLit >= 30 && m_nCntLit < 60)
				{ //30�t���[���ȏ�̂Ƃ�
					//�J���[�̍X�V
					SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else if (m_nCntLit == 60)
				{ //60�t���[���o�����Ƃ�
					//�_���J�E���g�̏�����
					m_nCntLit = 0;
				}

				CKeyboard *pKeyboard = CManager::GetKeyboard();

				if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || CManager::GetController()->GetJoypadTrigger(3))
				{ //Enter�L�[ �܂��� B�{�^�����������Ƃ�

					//SE�̍Đ�
					CManager::GetSound()->Play(CSound::SOUND_LABEL_DOOR_OPEN);

					switch (m_mode)
					{
					case CManager::MODE_TITLE:

						//�t�F�[�h�̐���
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_TITLE);

						break;

					case CManager::MODE_SELECT:

						//�t�F�[�h�̐���
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_SELECT);

						break;

					case CManager::MODE_GAME:

						//�t�F�[�h�̐���
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_GAME);

						break;

					case CManager::MODE_SCORE_ATTACK:

						//�t�F�[�h�̐���
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_SCORE_ATTACK);

						break;

					case CManager::MODE_TUTORIAL:

						//�t�F�[�h�̐���
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_TUTORIAL);

						break;

					case CManager::MODE_TUTORIAL_GAME:

						//�t�F�[�h�̐���
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_TUTORIAL_GAME);

						break;

					case CManager::MODE_TUTORIAL_SCORE_ATTACK:

						//�t�F�[�h�̐���
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_TUTORIAL_SCORE_ATTACK);

						break;

					case CManager::MODE_RANKING:

						//�t�F�[�h�̐���
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_RANKING);

						break;

					default:

						break;
					}
				}
			}
			else
			{
				//�J���[�̍X�V
				SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				//�_���J�E���g�̏�����
				m_nCntLit = 0;
			}

			break;
		}
	}

	//�V�[��2D�̍X�V
	CScene2D::Update();
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CDoor::Draw(void)
{
	//�V�[��2D�̕`�揈��
	CScene2D::Draw();
}