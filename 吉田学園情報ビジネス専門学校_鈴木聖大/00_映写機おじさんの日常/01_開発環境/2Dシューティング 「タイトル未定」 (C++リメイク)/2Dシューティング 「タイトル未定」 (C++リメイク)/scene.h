//=============================================================================
//
// オブジェクト処理 [scene.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_OBJECT (256)	//オブジェクトの最大数

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CScene	//オブジェクトクラス
{
public:
	//オブジェクトタイプの列挙体
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_BG,					//背景
		OBJTYPE_DOOR,				//ドア
		OBJTYPE_UI_BACK,			//後面のUI
		OBJTYPE_EFFECT_BULLET,		//弾のエフェクト
		OBJTYPE_BULLET,				//バレット
		OBJTYPE_FIRST_BALL,			//ファーストボール
		OBJTYPE_SECOND_BALL,		//セカンドボール
		OBJTYPE_THIRD_BALL,			//サードボール
		OBJTYPE_BOSS_BALL,			//ボスボール
		OBJTYPE_ITEM,				//アイテム
		OBJTYPE_SCORE,				//スコア
		OBJTYPE_PLAYER,				//プレイヤー
		OBJTYPE_BARRIER,			//バリア
		OBJTYPE_EFFECT,				//エフェクト
		OBJTYPE_EXPLOSION,			//爆発
		OBJTYPE_UI_FRONT,			//前面のUI
		OBJTYPE_UI_ITEM,			//アイテムのUI
		OBJTYPE_NUMBER,				//数字
		OBJTYPE_PAUSE,				//ポーズ
		OBJTYPE_DESCRIPTION,		//操作説明
		OBJTYPE_START_OR_FINISH,	//スタート時のカウント
		OBJTYPE_FADE,				//フェード
		OBJTYPE_MAX
	}OBJTYPE;

	CScene();
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static int m_nNumAll;
	static void ReleaseAll(void);
	static void ReleaseAllOtherThanFadeAndPlayer(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	static CScene *m_apScene[MAX_OBJECT];

	static void UpdateStop(bool bUpdateStop);
	void SetObjType(OBJTYPE objType);

	static bool GetUpdateStop(void);
	OBJTYPE GetObjType(void);
	static CScene *GetScene(int nCount);

protected:
	void Release(void);

private:
	int m_nID;
	OBJTYPE m_objType;	//オブジェクトの種類
	static bool m_bUpdateStop;
};

#endif