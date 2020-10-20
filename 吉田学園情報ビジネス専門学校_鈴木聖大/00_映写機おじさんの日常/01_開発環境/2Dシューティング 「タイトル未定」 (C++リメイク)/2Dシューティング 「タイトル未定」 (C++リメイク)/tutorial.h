//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "mode.h"

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CTutorial :public CMode
{
public:
	//チュートリアルのモードの列挙型
	typedef enum
	{
		TUTORIAL_MODE_NONE = 0,
		TUTORIAL_MODE_GAME,			//ステージモード
		TUTORIAL_MODE_SCORE_ATTACK,	//スコアアタックモード
		TUTORIAL_MODE_MAX
	}TUTORIAL_MODE;

	CTutorial();
	~CTutorial();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTutorialMode(TUTORIAL_MODE TutorialMode);

	static TUTORIAL_MODE GetTutorialMode(void);

private:
	static TUTORIAL_MODE m_TutorialMode;
};

#endif