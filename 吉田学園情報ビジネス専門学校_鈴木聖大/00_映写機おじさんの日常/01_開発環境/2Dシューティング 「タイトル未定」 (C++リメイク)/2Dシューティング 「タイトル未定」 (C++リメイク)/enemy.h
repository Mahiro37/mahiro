//================================================================
//
// 敵処理 [enemy.h]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//****************************************************************
//インクルードファイル
//****************************************************************
#include "scene2d.h"

//****************************************************************
//マクロ定義
//****************************************************************
#define BOSS_BALL_SIZE			(D3DXVECTOR3(200.0f,200.0f,0.0f))	//ボスボールのサイズ
#define FIRST_BALL_SIZE			(D3DXVECTOR3(150.0f,150.0f,0.0f))	//ファーストボールのサイズ
#define SECOND_BALL_SIZE		(D3DXVECTOR3(100.0f,100.0f,0.0f))	//セカンドボールのサイズ
#define THIRD_BALL_SIZE			(D3DXVECTOR3(50.0f,50.0f,0.0f))		//サードボールのサイズ
#define BOSS_BALL_RADIUS		(100.0f)							//ボスボールの半径
#define FIRST_BALL_RADIUS		(75.0f)								//ファーストボールの半径
#define SECOND_BALL_RADIUS		(50.0f)								//セカンドボールの半径
#define THIRD_BALL_RADIUS		(25.0f)								//サードボールの半径
#define BALL_DIVISION_POS		(D3DXVECTOR3(50.0f,50.0f,0.0f))		//分裂後の初期座標

//****************************************************************
//クラス定義
//****************************************************************
class CEnemy :public CScene2D
{
public:
	//ボールの種類の列挙型
	typedef enum
	{
		TYPE_FIRST = 0,	//ファーストボール
		TYPE_SECOND,	//セカンドボール
		TYPE_THIRD,		//サードボール
		TYPE_BOSS,		//ボスボール
		TYPE_MAX
	}TYPE;

	//ボールの状態の列挙型
	typedef enum
	{
		STATE_NONE = 0,	//通常
		STATE_DAMAGE	//ダメージ
	}ENEMY_STATE;

	CEnemy();
	~CEnemy();
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, OBJTYPE type);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, OBJTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move);
	void SetBallState(ENEMY_STATE state);
	void SetBallLife(int nDamage);
	void Reflection(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void RotationEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move);

	int GetBallLife(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];
	ENEMY_STATE m_EnemyState;
	D3DXVECTOR3 m_BarrierPosOld;
	D3DXVECTOR3 m_move;
	int m_nBarrierLife;
	int m_nBallLife;
	int m_nDamageColorChange;
	int m_nInvincibleTime;
	int m_nCntCreateBall;
};

#endif