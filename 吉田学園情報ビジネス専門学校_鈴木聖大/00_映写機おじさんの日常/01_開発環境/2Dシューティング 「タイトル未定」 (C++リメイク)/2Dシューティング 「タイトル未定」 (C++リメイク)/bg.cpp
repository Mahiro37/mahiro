//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"

//*****************************************************************************
//�ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBg::m_apTexture[MAX_BG_TEXTURE] = {};

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CBg::CBg()
{
	memset(m_pScene2D, 0, sizeof(m_pScene2D));
	m_nAnimCounter = 0;
	m_nAnimPattern = 0;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CBg::~CBg()
{
}

//*****************************************************************************
//���[�h����
//*****************************************************************************
HRESULT CBg::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm00.jpg", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm01.jpg", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm02.jpg", &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm03.jpg", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm04.jpg", &m_apTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm05.jpg", &m_apTexture[5]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm06.jpg", &m_apTexture[6]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm07.jpg", &m_apTexture[7]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm08.jpg", &m_apTexture[8]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm09.jpg", &m_apTexture[9]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm10.jpg", &m_apTexture[10]);

	return S_OK;
}

//*****************************************************************************
//�A�����[�h����
//*****************************************************************************
void CBg::Unload(void)
{
	//�e�N�X�`���̔j��
	for (int nCount = 0; nCount < MAX_BG_TEXTURE; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//�w�i�̐���
//*****************************************************************************
CBg *CBg::Create(void)
{
	//�w�i�̃������m��
	CBg *pBg = new CBg;

	//�w�i�̏�����
	pBg->Init();

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	pBg->SetObjType(OBJTYPE_BG);

	return pBg;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CBg::Init(void)
{
	for (int nCount = 0; nCount < MAX_BG; nCount++)
	{
		//�V�[��2D�̃������m��
		m_pScene2D[nCount] = new CScene2D;

		if (m_pScene2D[nCount] != NULL)
		{
			//�I�u�W�F�N�g�̎�ނ̐ݒ�
			m_pScene2D[nCount]->SetObjType(OBJTYPE_BG);

			//�T�C�Y�̐ݒ�
			m_pScene2D[nCount]->SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

			//�V�[��2D�̏���������
			m_pScene2D[nCount]->Init();

			//�e�N�X�`���̐ݒ�
			m_pScene2D[nCount]->BindTexture(m_apTexture[nCount]);
		}
	}

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CBg::Uninit(void)
{
	//�V�[��2D�̏I������
	for (int nCount = 0; nCount < MAX_BG; nCount++)
	{
		if (m_pScene2D[nCount] != NULL)
		{
			m_pScene2D[nCount]->Uninit();
			m_pScene2D[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CBg::Update(void)
{
	m_nAnimCounter++;

	if (m_nAnimCounter == 2)
	{
		m_nAnimCounter = 0;

		for (int nCount = 0; nCount < MAX_BG; nCount++)
		{
			if (m_pScene2D[nCount] != NULL)
			{
				m_nAnimPattern++;

				if (m_nAnimPattern == MAX_BG_TEXTURE)
				{
					m_nAnimPattern = 0;
				}

				//�e�N�X�`���̐ݒ�
				m_pScene2D[nCount]->BindTexture(m_apTexture[m_nAnimPattern]);
			}
		}
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CBg::Draw(void)
{
}