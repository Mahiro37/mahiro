//=============================================================================
//
// 選択処理 [select.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _SELECT_H_
#define _SELECT_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "mode.h"

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CSelect :public CMode
{
public:
	CSelect();
	~CSelect();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

};

#endif