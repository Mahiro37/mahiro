//=============================================================================
//
// UI処理 [ui.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2d.h"

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CUi:public CScene2D
{
public:
	//UIの種類の列挙型
	typedef enum
	{
		UI_TYPE_NONE = 0,
		UI_TYPE_TIMER,					//タイマー
		UI_TYPE_ZANKI,					//残機
		UI_TYPE_FILM,					//フィルム
		UI_TYPE_SCORE,					//スコア
		UI_TYPE_PLAYER_SPEED_UP,		//プレイヤーの移動スピードアップ
		UI_TYPE_BULLET_SPEED_UP,		//プレイヤーの弾のスピードアップ
		UI_TYPE_BARRIER,				//バリア
		UI_TYPE_RANKING_1,				//1位
		UI_TYPE_RANKING_2,				//2位
		UI_TYPE_RANKING_3,				//3位
		UI_TYPE_RANKING_4,				//4位
		UI_TYPE_RANKING_5,				//5位
		UI_TYPE_RANKING_6,				//6位
		UI_TYPE_RANKING_7,				//7位
		UI_TYPE_RANKING_8,				//8位
		UI_TYPE_RANKING_9,				//9位
		UI_TYPE_RANKING_10,				//10位
		UI_TYPE_START,					//スタート
		UI_TYPE_FINISH,					//フィニッシュ
		UI_TYPE_GAME_OVER,				//ゲームオーバー
		UI_TYPE_GAME_CLEAR,				//ゲームクリア
		UI_TYPE_STAGE_1,				//ステージ１
		UI_TYPE_STAGE_2,				//ステージ２
		UI_TYPE_STAGE_3,				//ステージ３
		UI_TYPE_GAME_SCORE_SCREEN,		//スコア表示画面
		UI_TYPE_RANK_IN,				//ランクイン
		UI_TYPE_SELECT,					//セレクト
		UI_TYPE_SELECT_TITLE,			//タイトルロゴ(ドア用)
		UI_TYPE_SELECT_GAME,			//ゲームロゴ(ドア用)
		UI_TYPE_SELECT_SCORE_ATTACK,	//スコアアタックロゴ(ドア用)
		UI_TYPE_SELECT_TUTORIAL,		//チュートリアルロゴ(ドア用)
		UI_TYPE_SELECT_RANKING,			//ランキングロゴ(ドア用)
		UI_TYPE_SELECT_SELECT,			//セレクトロゴ(ドア用)
		UI_TYPE_BALL_CREATE,			//ボールの生成場所
		UI_TYPE_TUTORIAL_GAME,			//チュートリアルのゲームモード表示
		UI_TYPE_TUTORIAL_SCORE_ATTACK,	//チュートリアルのスコアアタックモード表示
		UI_TYPE_FUKIDASHI,				//吹き出し
		UI_TYPE_RANKING_MESSAGE,		//ランキング部屋用のメッセージ
		UI_TYPE_DESCRIPTION,			//操作説明
		UI_TYPE_TITLE,					//ゲームタイトル
		UI_TYPE_CONTINUE,				//続ける(ポーズ)
		UI_TYPE_INSTRUCTION,			//操作説明(ポーズ)
		UI_TYPE_START_OVER,				//最初から(ポーズ)
		UI_TYPE_RETURN_TO_SELECT,		//セレクトへ(ポーズ)
		UI_TYPE_PAUSE,					//ポーズ背景
		UI_TYPE_MAX
	}UI_TYPE;

	CUi();
	~CUi();
	static HRESULT Load(void);
	static void Unload(void);
	static CUi *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI_TYPE type);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPauseChoice(bool bPauseChoice);
	void BindTexture(UI_TYPE type);

	bool GetPauseChoice(void);
	UI_TYPE GetUiType(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[UI_TYPE_MAX];
	UI_TYPE m_type;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_move;
	D3DXCOLOR m_color;
	int m_nCntColor;
	int m_nCntUi;
	bool m_bLogoMove;
	bool m_bTransparence;
	bool m_bPauseChoice;
};

#endif