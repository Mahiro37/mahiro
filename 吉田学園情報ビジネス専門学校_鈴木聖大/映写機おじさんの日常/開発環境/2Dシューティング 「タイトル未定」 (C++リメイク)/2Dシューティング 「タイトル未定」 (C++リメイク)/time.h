//=============================================================================
//
// �^�C������ [time.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

//*****************************************************************************
//�w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "number.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define TIME_NUM			(2)			//�^�C���̌���
#define TIME_START_NUM		(60)		//�J�n���̃^�C��
#define RESULT_TRANSITION	(60)		//���U���g�ɑJ�ڂ��鎞��

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CTime :public CScene
{
public:
	CTime();
	~CTime();
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, CNumber::TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTime(int nTime);
	void SaveTime(int nTime);
	void SetStopTime(bool bTimeStop);
	void ResetTotalTime(void);

	int GetTime(void);
	int GetTotalTime(void);
	int GetSaveTime(void);

private:
	static CNumber *m_pNumber[TIME_NUM];
	int m_nTime;
	int m_nTotalTime;
	int m_nSaveTime;
	int m_nTimeCounter;
	bool m_bTimeStop;
};

#endif