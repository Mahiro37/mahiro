//=============================================================================
//
// �o���A���� [barrier.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "barrier.h"
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "player.h"
#include "score.h"
#include "select.h"
#include "score_attack.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define ANIMATION_SPEED		(5)		//�A�j���[�V�����̑���
#define ANIMATION_ROW		(4)		//�A�j���[�V�����̍s��
#define ANIMATION_LINE		(5)		//�A�j���[�V�����̗�

//*****************************************************************************
//�ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBarrier::m_pTexture = NULL;
int CBarrier::m_nBarrierLife = 0;

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CBarrier::CBarrier()
{
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CBarrier::~CBarrier()
{
}

//*****************************************************************************
//���[�h����
//*****************************************************************************
HRESULT CBarrier::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect04.png", &m_pTexture);

	return S_OK;
}

//*****************************************************************************
//�A�����[�h����
//*****************************************************************************
void CBarrier::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//*****************************************************************************
//�G�t�F�N�g�̐���
//*****************************************************************************
CBarrier *CBarrier::Create(D3DXVECTOR3 pos)
{
	//�G�t�F�N�g�̃������m��
	CBarrier *pBarrier = new CBarrier;

	//�G�t�F�N�g�̏���������
	pBarrier->Init(pos);

	return pBarrier;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CBarrier::Init(D3DXVECTOR3 pos)
{
	//�|�W�V�����̐ݒ�
	SetPos(pos);

	//�T�C�Y�̐ݒ�
	SetSize(BARRIER_SIZE);

	//�J���[�̐ݒ�
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));

	//�G�t�F�N�g�e�N�X�`���̃A�j���[�V����
	InitAnimation(ANIMATION_SPEED, ANIMATION_ROW, ANIMATION_LINE);

	//�e�N�X�`���̐ݒ�
	BindTexture(m_pTexture);

	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(OBJTYPE_BARRIER);

	//���C�t�̏�����
	m_nBarrierLife = BARRIER_TYPE_1;

	//�V�[��2D�̏���������
	CScene2D::Init();

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CBarrier::Uninit(void)
{
	m_nBarrierLife = 0;

	//�V�[��2D�̏I������
	CScene2D::Uninit();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CBarrier::Update(void)
{
	//�v���C���[�|�W�V�����̎擾
	D3DXVECTOR3 PlayerPos = CManager::GetPlayer()->GetPos();

	//�|�W�V�����̐ݒ�
	SetPos(PlayerPos);

	//�J���[�̕ύX
	switch (m_nBarrierLife)
	{
	case BARRIER_TYPE_1:

		//�J���[�̐ݒ�
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
		
		break;

	case BARRIER_TYPE_2:

		//�J���[�̐ݒ�
		SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f));

		break;

	case BARRIER_TYPE_3:

		//�J���[�̐ݒ�
		SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.7f));

		break;

	case BARRIER_TYPE_SCORE:

		m_nBarrierLife = BARRIER_TYPE_3;

		break;

	default:

		break;
	}

	//�V�[��2D�̍X�V����
	CScene2D::Update();

	//�A�j���[�V�����̍X�V����
	CScene2D::UpdateAnimation(ANIMATION_REPEAT, NOT_INVERSION);
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CBarrier::Draw(void)
{
	//�V�[���̕`�揈��
	CScene2D::Draw();
}

//*****************************************************************************
//�o���A���C�t�̐ݒ�
//*****************************************************************************
void CBarrier::SetBarrierLife(int nBarrierLife)
{
	if (m_nBarrierLife != 0)
	{
		m_nBarrierLife += nBarrierLife;
	}
}

//*****************************************************************************
//�o���A���C�t�̎擾
//*****************************************************************************
int CBarrier::GetBarrierLife(void)
{
	return m_nBarrierLife;
}