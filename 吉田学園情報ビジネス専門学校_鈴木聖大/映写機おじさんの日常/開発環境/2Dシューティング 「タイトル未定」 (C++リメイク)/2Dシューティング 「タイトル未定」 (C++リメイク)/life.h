//=============================================================================
//
// ���C�t���� [life.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//*****************************************************************************
//�w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "number.h"

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CLife :public CScene
{
public:
	CLife();
	~CLife();
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetLife(int nLife);

private:
	static CNumber *m_pNumber;
	static int m_nLife;
};

#endif