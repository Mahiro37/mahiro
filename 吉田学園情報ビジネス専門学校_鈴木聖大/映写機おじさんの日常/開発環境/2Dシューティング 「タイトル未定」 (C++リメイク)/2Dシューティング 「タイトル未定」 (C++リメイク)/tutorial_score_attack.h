//=============================================================================
//
// チュートリアル処理(スコアアタックモード) [tutorial_score_attack.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _TUTORIAL_SCORE_ATTACK_H_
#define _TUTORIAL_SCORE_ATTACK_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "tutorial.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CScore;
class CUi;

//*****************************************************************************
//クラス定義
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