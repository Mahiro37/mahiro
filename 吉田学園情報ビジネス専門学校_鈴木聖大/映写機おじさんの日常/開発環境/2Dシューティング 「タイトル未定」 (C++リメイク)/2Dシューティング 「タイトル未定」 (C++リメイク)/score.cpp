//=============================================================================
//
// スコア処理 [score.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "score.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define NUMBER_INTERVAL		(40.0f)		//数字の間隔

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
int CScore::m_nScore = 0;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CScore::CScore()
{
	memset(m_apNumber, 0, sizeof(m_apNumber));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CScore::~CScore()
{
}

//*****************************************************************************
//スコアの生成
//*****************************************************************************
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fInterval, int nScore, CNumber::TYPE type)
{
	//スコアのメモリ確保
	CScore *pScore = new CScore;

	//スコアの初期化処理
	if (pScore != NULL)
	{
		pScore->Init(pos, size, fInterval, nScore, type);
	}

	return pScore;
}

HRESULT CScore::Init(void)
{
	return S_OK;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fInterval, int nScore, CNumber::TYPE type)
{
	//位置の設定
	m_pos = pos;

	for (int nCount = 0; nCount < SCORE_NUM; nCount++)
	{
		//数字のメモリ確保
		m_apNumber[nCount] = CNumber::Create(pos, size, 0, type);

		//数字の設定
		int nNum = (nScore / (int)(pow(10, nCount))) % 10;
		m_apNumber[nCount]->SetNumber(nNum);

		//オブジェクトの種類の設定
		SetObjType(OBJTYPE_SCORE);

		//数字の間隔をあける
		pos.x -= fInterval;
	}

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CScore::Uninit(void)
{
	for (int nCount = 0; nCount < SCORE_NUM; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Uninit();
			delete m_apNumber[nCount];
			m_apNumber[nCount] = NULL;
		}
	}

	//メモリの開放処理
	//Release();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CScore::Update(void)
{
	for (int nCount = 0; nCount < SCORE_NUM; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Update();
		}
	}
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CScore::Draw(void)
{
	for (int nCount = 0; nCount < SCORE_NUM; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Draw();
		}
	}
}

//*****************************************************************************
//スコアを半分にする処理
//*****************************************************************************
void CScore::HalfScore(void)
{
	double dScore = (double)m_nScore / 2;
	dScore = dScore - (int)dScore;

	if (dScore >= 0.5)
	{
		m_nScore = (m_nScore / 2) + 1;
	}
	else
	{
		m_nScore = m_nScore / 2;
	}

	//スコアの設定処理
	SetScore(m_nScore);
}

//*****************************************************************************
//スコアの加算処理
//*****************************************************************************
void CScore::AddScore(int nScore)
{
	m_nScore += nScore;

	for (int nCount = 0; nCount < SCORE_NUM; nCount++)
	{
		int nNum = (m_nScore / (int)(pow(10, nCount))) % 10;

		//数字の設定
		m_apNumber[nCount]->SetNumber(nNum);
	}
}

//*****************************************************************************
//スコアの設定処理
//*****************************************************************************
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;

	for (int nCount = 0; nCount < SCORE_NUM; nCount++)
	{
		int nNum = (m_nScore / (int)(pow(10, nCount))) % 10;

		//数字の設定
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->SetNumber(nNum);
		}
	}
}

//*****************************************************************************
//スコアの取得
//*****************************************************************************
int CScore::GetScore(void)
{
	return m_nScore;
}

//*****************************************************************************
//数字の取得
//*****************************************************************************
CNumber *CScore::GetNumber(int nNum)
{
	return m_apNumber[nNum];
}