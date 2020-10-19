//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "score.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define NUMBER_INTERVAL		(40.0f)		//�����̊Ԋu

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
int CScore::m_nScore = 0;

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CScore::CScore()
{
	memset(m_apNumber, 0, sizeof(m_apNumber));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CScore::~CScore()
{
}

//*****************************************************************************
//�X�R�A�̐���
//*****************************************************************************
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fInterval, int nScore, CNumber::TYPE type)
{
	//�X�R�A�̃������m��
	CScore *pScore = new CScore;

	//�X�R�A�̏���������
	if (pScore != NULL)
	{
		pScore->Init(pos, size, fInterval, nScore, type);
	}

	return pScore;
}

HRESULT CScore::Init(void)
{
	return S_OK;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fInterval, int nScore, CNumber::TYPE type)
{
	//�ʒu�̐ݒ�
	m_pos = pos;

	for (int nCount = 0; nCount < SCORE_NUM; nCount++)
	{
		//�����̃������m��
		m_apNumber[nCount] = CNumber::Create(pos, size, 0, type);

		//�����̐ݒ�
		int nNum = (nScore / (int)(pow(10, nCount))) % 10;
		m_apNumber[nCount]->SetNumber(nNum);

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_SCORE);

		//�����̊Ԋu��������
		pos.x -= fInterval;
	}

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CScore::Uninit(void)
{
	for (int nCount = 0; nCount < SCORE_NUM; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Uninit();
			delete m_apNumber[nCount];
			m_apNumber[nCount] = NULL;
		}
	}

	//�������̊J������
	//Release();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CScore::Update(void)
{
	for (int nCount = 0; nCount < SCORE_NUM; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Update();
		}
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CScore::Draw(void)
{
	for (int nCount = 0; nCount < SCORE_NUM; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Draw();
		}
	}
}

//*****************************************************************************
//�X�R�A�𔼕��ɂ��鏈��
//*****************************************************************************
void CScore::HalfScore(void)
{
	double dScore = (double)m_nScore / 2;
	dScore = dScore - (int)dScore;

	if (dScore >= 0.5)
	{
		m_nScore = (m_nScore / 2) + 1;
	}
	else
	{
		m_nScore = m_nScore / 2;
	}

	//�X�R�A�̐ݒ菈��
	SetScore(m_nScore);
}

//*****************************************************************************
//�X�R�A�̉��Z����
//*****************************************************************************
void CScore::AddScore(int nScore)
{
	m_nScore += nScore;

	for (int nCount = 0; nCount < SCORE_NUM; nCount++)
	{
		int nNum = (m_nScore / (int)(pow(10, nCount))) % 10;

		//�����̐ݒ�
		m_apNumber[nCount]->SetNumber(nNum);
	}
}

//*****************************************************************************
//�X�R�A�̐ݒ菈��
//*****************************************************************************
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;

	for (int nCount = 0; nCount < SCORE_NUM; nCount++)
	{
		int nNum = (m_nScore / (int)(pow(10, nCount))) % 10;

		//�����̐ݒ�
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->SetNumber(nNum);
		}
	}
}

//*****************************************************************************
//�X�R�A�̎擾
//*****************************************************************************
int CScore::GetScore(void)
{
	return m_nScore;
}

//*****************************************************************************
//�����̎擾
//*****************************************************************************
CNumber *CScore::GetNumber(int nNum)
{
	return m_apNumber[nNum];
}