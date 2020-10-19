//=============================================================================
//
// �`���[�g���A������(�X�R�A�A�^�b�N���[�h) [tutorial_score_attack.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _TUTORIAL_SCORE_ATTACK_H_
#define _TUTORIAL_SCORE_ATTACK_H_

//*****************************************************************************
//�w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "tutorial.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CScore;
class CUi;

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CTutorialScoreAttack :public CTutorial
{
public:
	CTutorialScoreAttack();
	~CTutorialScoreAttack();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void AddScore(int nNum);

private:
	static CScore *m_pScore;
	CUi* m_pDescriptionUi;
	bool m_bBall;
};

#endif