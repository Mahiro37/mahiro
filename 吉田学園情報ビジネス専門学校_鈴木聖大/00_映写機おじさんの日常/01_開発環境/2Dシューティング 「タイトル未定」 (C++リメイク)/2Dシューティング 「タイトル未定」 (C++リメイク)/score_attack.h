//=============================================================================
//
// スコアアタック処理 [score_attack.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _SCORE_ATTACK_H_
#define _SCORE_ATTACK_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "mode.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CScore;
class CMessage;
class CFade;

//*****************************************************************************
//クラス定義
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