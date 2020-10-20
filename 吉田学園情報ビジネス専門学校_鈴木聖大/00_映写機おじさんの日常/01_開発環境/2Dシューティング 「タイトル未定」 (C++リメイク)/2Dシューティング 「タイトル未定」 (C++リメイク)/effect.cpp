//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "effect.h"
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "player.h"
#include "bullet.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define EFFECT_SIZE			(50)	//エフェクトの大きさ
#define ANIMATION_SPEED		(5)		//アニメーションの速さ
#define ANIMATION_PATTERN	(8)		//アニメーションのパターン数
#define SHOT_EFFECT_LIFE	(10)	//チャージショットエフェクトのライフ

//*****************************************************************************
//静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEffect::m_pTexture[EFFECT_TYPE_MAX] = {};

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CEffect::CEffect()
{
	m_EffectType = EFFECT_TYPE_NONE;
	m_Animation = ANIMATION_UNREPEAT;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SetSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nLifeCounter = 0;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CEffect::~CEffect()
{
}

//*****************************************************************************
//ロード処理
//*****************************************************************************
HRESULT CEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect00.png", &m_pTexture[EFFECT_TYPE_GET_SCORE]);
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect01.png", &m_pTexture[EFFECT_TYPE_GET_SPEED_UP]);
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect02.png", &m_pTexture[EFFECT_TYPE_GET_TWIN_BULLET]);
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect03.png", &m_pTexture[EFFECT_TYPE_GET_BARRIER]);
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect04.png", &m_pTexture[EFFECT_TYPE_BARRIER]);
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect05.png", &m_pTexture[EFFECT_TYPE_CHARGE_SHOT]);

	return S_OK;
}

//*****************************************************************************
//アンロード処理
//*****************************************************************************
void CEffect::Unload(void)
{
	// テクスチャの破棄
	for (int nCount = 0; nCount < EFFECT_TYPE_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//エフェクトの生成
//*****************************************************************************
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fEffectSpeed, EFFECT_TYPE EffectType, ANIMATION Animation, CSound::SOUND_LABEL SoundLabel)
{
	//エフェクトのメモリ確保
	CEffect *pEffect = new CEffect;

	//エフェクトの初期化処理
	pEffect->Init(pos, size, fEffectSpeed, EffectType, Animation);

	switch (EffectType)
	{
	case EFFECT_TYPE_GET_SCORE:
	case EFFECT_TYPE_GET_SPEED_UP:
	case EFFECT_TYPE_GET_TWIN_BULLET:
	case EFFECT_TYPE_GET_BARRIER:

		//サウンドの再生
		CManager::GetSound()->Play(SoundLabel);

		break;

	default:

		break;
	}

	return pEffect;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fEffectSpeed, EFFECT_TYPE EffectType, ANIMATION Animation)
{
	//エフェクトタイプの設定
	m_EffectType = EffectType;

	//アニメーションの設定
	m_Animation = Animation;

	//プレイヤーポジションの取得
	D3DXVECTOR3 PlayerPos = CManager::GetPlayer()->GetPos();

	//位置の設定
	m_pos = pos;

	//サイズの設定
	SetSize(size);

	switch (m_EffectType)
	{
	case EFFECT_TYPE_GET_SCORE:

		//位置の設定
		SetPos(pos + PlayerPos);

		//エフェクトテクスチャのアニメーション
		InitAnimation(fEffectSpeed, 5.0f, 3.0f);

		//オブジェクトの種類の設定
		SetObjType(OBJTYPE_EFFECT);

		break;

	case EFFECT_TYPE_GET_SPEED_UP:

		//位置の設定
		SetPos(pos + PlayerPos);

		//エフェクトテクスチャのアニメーション
		InitAnimation(fEffectSpeed, 7.0f, 7.0f);

		//オブジェクトの種類の設定
		SetObjType(OBJTYPE_EFFECT);

		break;

	case EFFECT_TYPE_GET_TWIN_BULLET:

		//位置の設定
		SetPos(pos + PlayerPos);

		//エフェクトテクスチャのアニメーション
		InitAnimation(fEffectSpeed, 7.0f, 7.0f);

		//オブジェクトの種類の設定
		SetObjType(OBJTYPE_EFFECT);

		break;

	case EFFECT_TYPE_GET_BARRIER:

		//位置の設定
		SetPos(pos + PlayerPos);

		//エフェクトテクスチャのアニメーション
		InitAnimation(fEffectSpeed, 7.0f, 7.0f);

		//オブジェクトの種類の設定
		SetObjType(OBJTYPE_EFFECT);

		break;

	case EFFECT_TYPE_CHARGE_SHOT:

		//位置の設定
		SetPos(pos);

		//初期サイズの設定
		m_size = size;

		//変更サイズの設定
		m_SetSize = size;

		//初期カラーの設定
		m_color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//オブジェクトの種類の設定
		SetObjType(OBJTYPE_EFFECT_BULLET);

		//サイズの設定
		SetSize(m_size);

		break;

	default:

		break;
	}

	//テクスチャの設定
	BindTexture(m_pTexture[EffectType]);

	//シーン2Dの初期化処理
	CScene2D::Init();

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CEffect::Uninit(void)
{
	//シーン2Dの終了処理
	CScene2D::Uninit();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CEffect::Update(void)
{
	//プレイヤーポジションの取得
	D3DXVECTOR3 PlayerPos = CManager::GetPlayer()->GetPos();

	switch (m_EffectType)
	{
	case EFFECT_TYPE_GET_SCORE:
	case EFFECT_TYPE_GET_SPEED_UP:
	case EFFECT_TYPE_GET_TWIN_BULLET:
	case EFFECT_TYPE_GET_BARRIER:

		//ポジションの更新
		SetPos(m_pos + PlayerPos);

		break;

	case EFFECT_TYPE_CHARGE_SHOT:

		m_nLifeCounter++;

		//α値とサイズを下げる
		m_color.a -= (1.0f / (float)SHOT_EFFECT_LIFE);
		m_SetSize -= (m_size / (float)SHOT_EFFECT_LIFE);

		//カラーの設定
		SetColor(m_color);

		//サイズの設定
		SetSize(m_SetSize);

		//見えなくなったとき
		if (m_nLifeCounter >= SHOT_EFFECT_LIFE)
		{
			m_nLifeCounter = 0;

			//終了処理
			Uninit();
			return;
		}

		break;

	default:

		break;
	}

	//シーン2Dの更新処理
	CScene2D::Update();

	//アニメーションの更新処理
	CScene2D::UpdateAnimation(m_Animation, NOT_INVERSION);
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	switch (m_EffectType)
	{
	case EFFECT_TYPE_CHARGE_SHOT:

		pDevice = CManager::GetRenderer()->GetDevice();

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//シーン2Dの描画処理
		CScene2D::Draw();

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		break;

	default:

		//シーンの描画処理
		CScene2D::Draw();

		break;
	}
}

//*****************************************************************************
//アニメーションの設定
//*****************************************************************************
void CEffect::SetAnimation(ANIMATION Animation)
{
	m_Animation = Animation;
}