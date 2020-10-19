//=============================================================================
//
// タイム処理 [time.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "number.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define TIME_NUM			(2)			//タイムの桁数
#define TIME_START_NUM		(60)		//開始時のタイム
#define RESULT_TRANSITION	(60)		//リザルトに遷移する時間

//*****************************************************************************
//クラス定義
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