//=============================================================================
//
// スコア処理 [score.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "number.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define SCORE_NUM	(5)		//スコアの桁数

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CScore :public CScene
{
public:
	CScore();
	~CScore();
	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fInterval, int nScore, CNumber::TYPE type);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fInterval, int nScore, CNumber::TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void HalfScore(void);
	void AddScore(int nScore);
	void SetScore(int nScore);

	int GetScore(void);
	CNumber *GetNumber(int nNum);

private:
	CNumber *m_apNumber[SCORE_NUM];
	D3DXVECTOR3 m_pos;
	static int m_nScore;
};

#endif