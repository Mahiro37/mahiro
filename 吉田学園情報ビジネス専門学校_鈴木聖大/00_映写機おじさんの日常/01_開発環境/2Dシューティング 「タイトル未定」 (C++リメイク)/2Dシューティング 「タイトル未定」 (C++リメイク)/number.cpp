//=============================================================================
//
// �����̏��� [number.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "number.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define NUM_VERTEX	(4)		//���_��
#define NUM_POLYGON (2)		//�|���S�� (�O�p�`�̐�)
#define NUMBER_NUM	(10)	//�����̐�

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DDEVICE9 CNumber::m_pDevice = NULL;
LPDIRECT3DTEXTURE9 CNumber::m_pTexture[TYPE_MAX] = {};

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CNumber::CNumber()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNumber = 0;
	m_type = TYPE_1;
	m_pVtxBuff = NULL;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CNumber::~CNumber()
{
}

//*****************************************************************************
//���[�h����
//*****************************************************************************
HRESULT CNumber::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "Textures/number/number00.png", &m_pTexture[TYPE_1]);
	D3DXCreateTextureFromFile(pDevice, "Textures/number/number01.png", &m_pTexture[TYPE_2]);

	return S_OK;
}

//*****************************************************************************
//�A�����[�h����
//*****************************************************************************
void CNumber::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//�����̐���
//*****************************************************************************
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumber, TYPE type)
{
	//�����̃������m��
	CNumber *pNumber = new CNumber;

	if (pNumber != NULL)
	{
		//�����̏���������
		pNumber->Init(pos, size, nNumber, type);
	}

	return pNumber;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumber, TYPE type)
{
	m_pos = pos;
	m_size = size;
	m_nNumber = nNumber;
	m_type = type;

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber + (1.0f / NUMBER_NUM), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber + (1.0f / NUMBER_NUM), 1.0f);

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CNumber::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CNumber::Update(void)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �|���S���̈ʒu��ݒ�
	pVtx[0].pos = D3DXVECTOR3((m_pos.x - (m_size.x / 2)), (m_pos.y - (m_size.y / 2)), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((m_pos.x + (m_size.x / 2)), (m_pos.y - (m_size.y / 2)), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((m_pos.x - (m_size.x / 2)), (m_pos.y + (m_size.y / 2)), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((m_pos.x + (m_size.x / 2)), (m_pos.y + (m_size.y / 2)), 0.0f);

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CNumber::Draw(void)
{
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	m_pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	m_pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	m_pDevice->SetTexture(0, m_pTexture[m_type]);

	// �|���S���̕`��
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//*****************************************************************************
//�ݒ菈��
//*****************************************************************************
void CNumber::SetNumber(int nNumber)
{
	m_nNumber = nNumber;

	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber + (1.0f / NUMBER_NUM), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber + (1.0f / NUMBER_NUM), 1.0f);

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
//�T�C�Y�̐ݒ�
//*****************************************************************************
void CNumber::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//*****************************************************************************
//�|�W�V�����̐ݒ�
//*****************************************************************************
void CNumber::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//*****************************************************************************
//�|�W�V�����̎擾
//*****************************************************************************
D3DXVECTOR3 CNumber::GetPos(void)
{
	return m_pos;
}