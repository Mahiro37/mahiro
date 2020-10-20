//=============================================================================
//
// �I�u�W�F�N�g���� [scene.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
int CScene::m_nNumAll = 0;
CScene *CScene::m_apScene[MAX_OBJECT] = {};
bool CScene::m_bUpdateStop = false;

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CScene::CScene()
{
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (m_apScene[nCount] == NULL)
		{
			m_apScene[nCount] = this;	//this : new�����|�C���^
			m_nID = nCount;				//���Ԗڂ��L�^
			m_nNumAll++;				//�������J�E���g

			break;
		}
	}
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CScene::~CScene()
{
}

//*****************************************************************************
//�S�Ẵ������̊J������
//*****************************************************************************
void CScene::ReleaseAll(void)
{
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (m_apScene[nCount] != NULL)
		{
			//���������m�ۂ���Ă����珈��������
			m_apScene[nCount]->Uninit();
		}
	}
}

//*****************************************************************************
//�t�F�[�h�ƃv���C���[�ȊO�̃������̊J������
//*****************************************************************************
void CScene::ReleaseAllOtherThanFadeAndPlayer(void)
{
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (m_apScene[nCount] != NULL)
		{
			if (m_apScene[nCount]->GetObjType() != OBJTYPE_FADE)
			{
				//���������m�ۂ���Ă����珈��������
				m_apScene[nCount]->Uninit();
			}
		}
	}
}

//*****************************************************************************
//�S�Ă̍X�V����
//*****************************************************************************
void CScene::UpdateAll(void)
{
	if (m_bUpdateStop == false)
	{
		for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
		{
			if (m_apScene[nCount] != NULL)
			{
				//���������m�ۂ���Ă����珈��������
				m_apScene[nCount]->Update();
			}
		}
	}
	else if (m_bUpdateStop == true)
	{
		for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
		{
			if (m_apScene[nCount] != NULL)
			{
				if (m_apScene[nCount]->GetObjType() == OBJTYPE_START_OR_FINISH || m_apScene[nCount]->GetObjType() == OBJTYPE_FADE || m_apScene[nCount]->GetObjType() == OBJTYPE_BG || m_apScene[nCount]->GetObjType() == OBJTYPE_PAUSE)
				{
					//���������m�ۂ���Ă����珈��������
					m_apScene[nCount]->Update();
				}
			}
		}
	}
}

//*****************************************************************************
//�S�Ă̕`�揈��
//*****************************************************************************
void CScene::DrawAll(void)
{
	for (int nCountType = 0; nCountType < OBJTYPE_MAX; nCountType++)
	{
		for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
		{
			if (m_apScene[nCount] != NULL && m_apScene[nCount]->GetObjType() == nCountType)
			{
				//���������m�ۂ���Ă����珈��������
				m_apScene[nCount]->Draw();
			}
		}
	}
}

//*****************************************************************************
//�������̊J������
//*****************************************************************************
void CScene::Release(void)
{
	if (m_apScene[m_nID] != NULL)
	{
		int nID = m_nID;	//delete������m_ID�̏�񂪏�����̂ŕϐ��ɓ����

		//m_apScene���������J��
		delete m_apScene[nID];
		m_apScene[nID] = NULL;

		m_nNumAll--;
	}
}

//*****************************************************************************
//�X�V���ꎞ�I�Ɏ~�߂鏈��
//*****************************************************************************
void CScene::UpdateStop(bool bUpdateStop)
{
	m_bUpdateStop = bUpdateStop;
}

//*****************************************************************************
//�I�u�W�F�N�g�^�C�v�̐ݒ�
//*****************************************************************************
void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

//*****************************************************************************
//�X�V���~�܂��Ă��邩���擾
//*****************************************************************************
bool CScene::GetUpdateStop(void)
{
	return m_bUpdateStop;
}

//*****************************************************************************
//�I�u�W�F�N�g�^�C�v�̎擾
//*****************************************************************************
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}

//*****************************************************************************
//�V�[���̎擾
//*****************************************************************************
CScene *CScene::GetScene(int nCount)
{
	return m_apScene[nCount];
}