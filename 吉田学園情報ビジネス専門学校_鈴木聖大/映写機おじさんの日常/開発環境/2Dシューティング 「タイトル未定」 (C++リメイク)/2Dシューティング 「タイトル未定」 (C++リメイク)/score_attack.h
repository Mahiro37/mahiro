//=============================================================================
//
// �X�R�A�A�^�b�N���� [score_attack.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _SCORE_ATTACK_H_
#define _SCORE_ATTACK_H_

//*****************************************************************************
//�w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "mode.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CScore;
class CMessage;
class CFade;

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CScoreAttack :public CMode
{
public:
	CScoreAttack();
	~CScoreAttack();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void AddScore(int nNum);

	static CScore *GetScore(void);

private:
	static CScore *m_pScore;
	CMessage *m_pMessage;
	CFade *m_pFade;
	int m_nTime;
	int m_nCntResult;
};

#endif