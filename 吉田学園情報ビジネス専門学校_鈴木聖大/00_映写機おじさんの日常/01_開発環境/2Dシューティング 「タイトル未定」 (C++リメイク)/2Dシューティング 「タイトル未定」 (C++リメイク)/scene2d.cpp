//=============================================================================
//
// �I�u�W�F�N�g���� [scene2d.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "scene2d.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)						//���_��
#define NUM_POLYGON (2)						//�|���S�� (�O�p�`�̐�)

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DDEVICE9 CScene2D::m_pDevice = NULL;

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CScene2D::CScene2D()
{
	m_pos = D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVtxBuff = NULL;
	m_pTexture = NULL;
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	m_nCounterLine = 0;
	m_fAnimationSpeed = 0;
	m_fAnimationPattern = 1;
	m_fAnimationLine = 1;
	m_fRadian = 0.0f;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CScene2D::~CScene2D()
{
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CScene2D::Init(void)
{
	VERTEX_2D *pVtx;

	m_pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@����
	m_pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,					// �o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,								// (�Œ�)
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,								// (�Œ�)
		&m_pVtxBuff,									// �ϐ������ς��ƕύX���K�v
		NULL);

	// ���_�f�[�^�͈̔͂����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �|���S���̈ʒu��ݒ�
	pVtx[0].pos = D3DXVECTOR3((m_pos.x - (m_size.x / 2)), (m_pos.y - (m_size.y / 2)), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((m_pos.x + (m_size.x / 2)), (m_pos.y - (m_size.y / 2)), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((m_pos.x - (m_size.x / 2)), (m_pos.y + (m_size.y / 2)), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((m_pos.x + (m_size.x / 2)), (m_pos.y + (m_size.y / 2)), 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[1].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[2].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[3].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_fAnimationPattern), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / m_fAnimationLine));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_fAnimationPattern), (1.0f / m_fAnimationLine));

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	//�I�u�W�F�N�g�j��
	Release();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CScene2D::Update(void)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �|���S�����W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[1].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[2].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[3].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CScene2D::Draw(void)
{
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	m_pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	m_pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	m_pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//*****************************************************************************
//�A�j���[�V�����̏���������
//*****************************************************************************
void CScene2D::InitAnimation(float fCounterAnim, float fPatternAnim, float fLineAnim)
{
	m_fAnimationSpeed = fCounterAnim;
	m_fAnimationPattern = fPatternAnim;
	m_fAnimationLine = fLineAnim;
}

//*****************************************************************************
//�A�j���[�V�����̍X�V����
//*****************************************************************************
void CScene2D::UpdateAnimation(ANIMATION AnimConditions, TEX texType)
{
	VERTEX_2D *pVtx;

	//�J�E���^�[�̉��Z
	m_nCounterAnim++;

	if (m_nCounterAnim == m_fAnimationSpeed)	//�A�j���[�V�����̑���
	{
		//�J�E���^�[�̃N���A
		m_nCounterAnim = 0;

		//�s���̉��Z
		m_nPatternAnim++;
	}

	if (m_nPatternAnim == m_fAnimationPattern)	//�A�j���[�V�����̍s��
	{
		//�s���̃N���A
		m_nPatternAnim = 0;

		//�񐔂̉��Z
		m_nCounterLine++;
	}

	if (m_nCounterLine == m_fAnimationLine)	//�A�j���[�V�����̗�
	{
		//�񐔂̃N���A
		m_nCounterLine = 0;

		//�A�j���[�V�������J��Ԃ��Ȃ��Ƃ�
		if (AnimConditions == ANIMATION_UNREPEAT)
		{
			//�I������
			Uninit();
		}

	}
	else
	{
		// ���_�f�[�^�͈̔͂����b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		switch (texType)
		{
		case NOT_INVERSION:

			//�e�N�X�`�����W�̍X�V
			pVtx[0].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern),									(float)m_nCounterLine * (1.0f / m_fAnimationLine));
			pVtx[1].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern) + (1.0f / m_fAnimationPattern),	(float)m_nCounterLine * (1.0f / m_fAnimationLine));
			pVtx[2].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern),									(float)m_nCounterLine * (1.0f / m_fAnimationLine) + (1.0f / m_fAnimationLine));
			pVtx[3].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern) + (1.0f / m_fAnimationPattern),	(float)m_nCounterLine * (1.0f / m_fAnimationLine) + (1.0f / m_fAnimationLine));

			break;

		case INVERSION:

			//�e�N�X�`�����W�̍X�V
			pVtx[0].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern),									(float)m_nCounterLine * (1.0f / m_fAnimationLine));
			pVtx[1].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern) - (1.0f / m_fAnimationPattern),	(float)m_nCounterLine * (1.0f / m_fAnimationLine));
			pVtx[2].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern),									(float)m_nCounterLine * (1.0f / m_fAnimationLine) + (1.0f / m_fAnimationLine));
			pVtx[3].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern) - (1.0f / m_fAnimationPattern),	(float)m_nCounterLine * (1.0f / m_fAnimationLine) + (1.0f / m_fAnimationLine));

			break;
		}

		// ���_�f�[�^���A�����b�N
		m_pVtxBuff->Unlock();
	}
}

//*****************************************************************************
//��]����
//*****************************************************************************
void CScene2D::Rotation(D3DXVECTOR3 pos, D3DXVECTOR3 size, ROTATION rotation)
{
	VERTEX_2D *pVtx;

	float fRollX1;
	float fRollX2;
	float fRollY1;
	float fRollY2;

	//�񂷊p�x�̉��Z
	m_fRadian += 0.05f;

	//���S���璸�_�܂ł̋����𑪂�i���F���S���璸�_�܂ł̋����j
	float r = sqrtf((size.x / 2) * (size.x / 2) + (size.y / 2) * (size.y / 2));

	//�Ȃ��p�Ƃ����߂�i�ƁF�Ȃ��p�j
	float fTheta = (float)atan2((size.y / 2), (size.x / 2));

	//��]�̌���
	switch (rotation)
	{
	case ROTATION_RIGHT:	//�E����

		//�����p�^�[�����쐬�iX�̑����AY�̑����j
		fRollX1 = cosf(fTheta + m_fRadian) * r;
		fRollX2 = cosf(fTheta - m_fRadian) * r;
		fRollY1 = sinf(fTheta + m_fRadian) * r;
		fRollY2 = sinf(fTheta - m_fRadian) * r;

		break;

	case ROTATION_LEFT:		//������

		//�����p�^�[�����쐬�iX�̑����AY�̑����j
		fRollX1 = cosf(fTheta - m_fRadian) * r;
		fRollX2 = cosf(fTheta + m_fRadian) * r;
		fRollY1 = sinf(fTheta - m_fRadian) * r;
		fRollY2 = sinf(fTheta + m_fRadian) * r;

		break;

	default:

		break;
	}

	//���_�f�[�^�͈̔͂����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�|���S�����W�̍X�V
	pVtx[0].pos = D3DXVECTOR3((pos.x - fRollX1), (pos.y - fRollY1), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((pos.x + fRollX2), (pos.y - fRollY2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((pos.x - fRollX2), (pos.y + fRollY2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((pos.x + fRollX1), (pos.y + fRollY1), 0.0f);

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
//�T�C�Y�̐ݒ�
//*****************************************************************************
void CScene2D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//*****************************************************************************
//�|�W�V�����̐ݒ�
//*****************************************************************************
void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//*****************************************************************************
//�J���[�̐ݒ�
//*****************************************************************************
void CScene2D::SetColor(D3DXCOLOR color)
{
	m_color = color;
}

//*****************************************************************************
//�e�N�X�`���̊��蓖��
//*****************************************************************************
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//*****************************************************************************
//�|�W�V�����̎擾
//*****************************************************************************
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;
}