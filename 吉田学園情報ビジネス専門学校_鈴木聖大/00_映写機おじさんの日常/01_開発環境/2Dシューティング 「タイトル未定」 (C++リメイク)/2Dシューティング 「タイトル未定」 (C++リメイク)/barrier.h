//================================================================
//
// �o���A���� [barrier.h]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _BARRIER_H_
#define _BARRIER_H_

//****************************************************************
//�C���N���[�h�t�@�C��
//****************************************************************
#include "scene2d.h"

//****************************************************************
//�}�N����`
//****************************************************************
#define BARRIER_SIZE	(D3DXVECTOR3(150.0f,150.0f,0.0f))	//�o���A�̃T�C�Y
#define BARRIER_RADIUS	(75.0f)								//�o���A�̔��a

//****************************************************************
//�N���X��`
//****************************************************************
class CBarrier :public CScene2D
{
public:
	//�o���A�̃^�C�v
	typedef enum
	{
		BARRIER_TYPE_NONE = 0,
		BARRIER_TYPE_1,
		BARRIER_TYPE_2,
		BARRIER_TYPE_3,
		BARRIER_TYPE_SCORE,
		BARRIER_MAX
	}BARRIER_TYPE;

	CBarrier();
	~CBarrier();
	static CBarrier *Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetBarrierLife(int nBarrierLife);

	int GetBarrierLife(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	static int m_nBarrierLife;
};

#endif