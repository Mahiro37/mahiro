//=============================================================================
//
// �����L���O���� [ranking.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
//�w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "mode.h"
#include "tcp_client.h"
#include "protocol_ranking.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CScore;
class CUi;

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CRanking :public CMode
{
public:
	CRanking();
	~CRanking();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static int SetRanking(int nScore);

	void GetRanking(void);

private:
	static CProtocolRanking::RankingData m_aRankingData[MAX_RANKING];
	CScore *m_apScore[MAX_RANKING];
	CUi *m_apUi[MAX_RANKING];
	int m_nCntScore;
	bool m_bUpdateScore;
};

#endif