//=============================================================================
//
// スコア結果処理 [ScoreScreen.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _SCORESCREEN_H_
#define _SCORESCREEN_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "mode.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CScore;

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CScoreScreen :public CMode
{
public:
	CScoreScreen();
	~CScoreScreen();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SaveScore(int nScore);
	static void SaveTotalTime(int nTotalTime);
	static void SavePlayerLife(int nPlayerLife);

private:
	CScore *m_pScore[4];
	static int m_nScore;
	static int m_nTotalTime;
	static int m_nRanking;
	static int m_nScoreBonus;
	static int m_nPlayerLife;
	int m_nTotalScore;
	int m_nPlayerLifeBonus;
	float m_fAddNum;
	bool m_bSet[4];
	int m_nCntScoreMove;
};

#endif