//=============================================================================
//
// バリア処理 [barrier.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "barrier.h"
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "player.h"
#include "score.h"
#include "select.h"
#include "score_attack.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define ANIMATION_SPEED		(5)		//アニメーションの速さ
#define ANIMATION_ROW		(4)		//アニメーションの行数
#define ANIMATION_LINE		(5)		//アニメーションの列数

//*****************************************************************************
//静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBarrier::m_pTexture = NULL;
int CBarrier::m_nBarrierLife = 0;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CBarrier::CBarrier()
{
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CBarrier::~CBarrier()
{
}

//*****************************************************************************
//ロード処理
//*****************************************************************************
HRESULT CBarrier::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect04.png", &m_pTexture);

	return S_OK;
}

//*****************************************************************************
//アンロード処理
//*****************************************************************************
void CBarrier::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//*****************************************************************************
//エフェクトの生成
//*****************************************************************************
CBarrier *CBarrier::Create(D3DXVECTOR3 pos)
{
	//エフェクトのメモリ確保
	CBarrier *pBarrier = new CBarrier;

	//エフェクトの初期化処理
	pBarrier->Init(pos);

	return pBarrier;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CBarrier::Init(D3DXVECTOR3 pos)
{
	//ポジションの設定
	SetPos(pos);

	//サイズの設定
	SetSize(BARRIER_SIZE);

	//カラーの設定
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));

	//エフェクトテクスチャのアニメーション
	InitAnimation(ANIMATION_SPEED, ANIMATION_ROW, ANIMATION_LINE);

	//テクスチャの設定
	BindTexture(m_pTexture);

	//オブジェクトの種類の設定
	SetObjType(OBJTYPE_BARRIER);

	//ライフの初期化
	m_nBarrierLife = BARRIER_TYPE_1;

	//シーン2Dの初期化処理
	CScene2D::Init();

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CBarrier::Uninit(void)
{
	m_nBarrierLife = 0;

	//シーン2Dの終了処理
	CScene2D::Uninit();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CBarrier::Update(void)
{
	//プレイヤーポジションの取得
	D3DXVECTOR3 PlayerPos = CManager::GetPlayer()->GetPos();

	//ポジションの設定
	SetPos(PlayerPos);

	//カラーの変更
	switch (m_nBarrierLife)
	{
	case BARRIER_TYPE_1:

		//カラーの設定
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
		
		break;

	case BARRIER_TYPE_2:

		//カラーの設定
		SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f));

		break;

	case BARRIER_TYPE_3:

		//カラーの設定
		SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.7f));

		break;

	case BARRIER_TYPE_SCORE:

		m_nBarrierLife = BARRIER_TYPE_3;

		break;

	default:

		break;
	}

	//シーン2Dの更新処理
	CScene2D::Update();

	//アニメーションの更新処理
	CScene2D::UpdateAnimation(ANIMATION_REPEAT, NOT_INVERSION);
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CBarrier::Draw(void)
{
	//シーンの描画処理
	CScene2D::Draw();
}

//*****************************************************************************
//バリアライフの設定
//*****************************************************************************
void CBarrier::SetBarrierLife(int nBarrierLife)
{
	if (m_nBarrierLife != 0)
	{
		m_nBarrierLife += nBarrierLife;
	}
}

//*****************************************************************************
//バリアライフの取得
//*****************************************************************************
int CBarrier::GetBarrierLife(void)
{
	return m_nBarrierLife;
}