//=============================================================================
//
// マネジメント処理 [manager.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "number.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CRenderer;
class CKeyboard;
class CController;
class CSound;
class CMode;
class CTime;
class CLife;
class CPlayer;

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CManager
{
public:
	//画面遷移の列挙型
	typedef enum
	{
		MODE_NONE = 0,
		MODE_TITLE,					//タイトル
		MODE_SELECT,				//セレクト
		MODE_GAME,					//ゲーム
		MODE_GAME_CREAR,			//ゲームクリア画面
		MODE_SCORE_ATTACK,			//スコアアタック
		MODE_SCORE_SCREEN,			//スコア表示画面
		MODE_TUTORIAL,				//チュートリアル
		MODE_TUTORIAL_GAME,			//チュートリアル(ゲームモード)
		MODE_TUTORIAL_SCORE_ATTACK,	//チュートリアル(スコアアタックモード)
		MODE_RANKING,				//ランキング
		MODE_MAX
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void CreateRenderer(HWND hWnd);
	static HRESULT CreateKeyboard(HINSTANCE hInstance, HWND hWnd);
	static HRESULT CreateController(HINSTANCE hInstance, HWND hWnd);
	static void CreateSound(HWND hWnd);
	static void CreateTime(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, CNumber::TYPE type);
	static void CreateLife(D3DXVECTOR3 pos);
	static void CreatePlayer(void);

	static CRenderer *GetRenderer(void);
	static CKeyboard *GetKeyboard(void);
	static CController *GetController(void);
	static CSound *GetSound(void);
	static CLife *GetLife(void);
	static CTime *GetTime(void);
	static CPlayer *GetPlayer(void);
	static MODE GetMode(void);
	static CMode *GetModePointer(void);

	void SetMode(MODE mode);

private:
	static CRenderer *m_pRenderer;
	static CKeyboard *m_pKeyboard;
	static CController *m_pController;
	static CSound *m_pSound;
	static CMode *m_pMode;
	static CLife *m_pLife;
	static CTime *m_pTime;
	static CPlayer *m_pPlayer;
	static MODE m_mode;
};

#endif