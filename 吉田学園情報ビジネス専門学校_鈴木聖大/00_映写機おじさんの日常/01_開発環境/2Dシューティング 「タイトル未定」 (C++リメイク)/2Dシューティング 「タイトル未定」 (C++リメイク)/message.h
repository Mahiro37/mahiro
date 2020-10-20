//=============================================================================
//
// ���b�Z�[�W�\������ [message.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

//*****************************************************************************
//�w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "ui.h"
#include "manager.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CFade;

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CMessage :public CScene
{
public:
	CMessage();
	~CMessage();
	static CMessage *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, CUi::UI_TYPE type, CManager::MODE mode);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, CUi::UI_TYPE type, CManager::MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CUi *m_pUi;
	CFade *m_pFade;
	static int m_nCounter;
	int m_nTime;
	CManager::MODE m_mode;
};

#endif