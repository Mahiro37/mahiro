//=============================================================================
//
// オブジェクト処理 [scene2d.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "scene2d.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)						//頂点数
#define NUM_POLYGON (2)						//ポリゴン (三角形の数)

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 CScene2D::m_pDevice = NULL;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CScene2D::CScene2D()
{
	m_pos = D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVtxBuff = NULL;
	m_pTexture = NULL;
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	m_nCounterLine = 0;
	m_fAnimationSpeed = 0;
	m_fAnimationPattern = 1;
	m_fAnimationLine = 1;
	m_fRadian = 0.0f;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CScene2D::~CScene2D()
{
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CScene2D::Init(void)
{
	VERTEX_2D *pVtx;

	m_pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファ生成
	m_pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,					// バッファサイズ
		D3DUSAGE_WRITEONLY,								// (固定)
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,								// (固定)
		&m_pVtxBuff,									// 変数名が変わると変更が必要
		NULL);

	// 頂点データの範囲をロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの位置を設定
	pVtx[0].pos = D3DXVECTOR3((m_pos.x - (m_size.x / 2)), (m_pos.y - (m_size.y / 2)), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((m_pos.x + (m_size.x / 2)), (m_pos.y - (m_size.y / 2)), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((m_pos.x - (m_size.x / 2)), (m_pos.y + (m_size.y / 2)), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((m_pos.x + (m_size.x / 2)), (m_pos.y + (m_size.y / 2)), 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[1].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[2].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[3].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_fAnimationPattern), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / m_fAnimationLine));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_fAnimationPattern), (1.0f / m_fAnimationLine));

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	//オブジェクト破棄
	Release();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CScene2D::Update(void)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴン座標の更新
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y - (m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[1].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[2].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);
	pVtx[3].col = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_color.a);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CScene2D::Draw(void)
{
	// 頂点バッファをデータストリームに設定
	m_pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	m_pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	m_pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//*****************************************************************************
//アニメーションの初期化処理
//*****************************************************************************
void CScene2D::InitAnimation(float fCounterAnim, float fPatternAnim, float fLineAnim)
{
	m_fAnimationSpeed = fCounterAnim;
	m_fAnimationPattern = fPatternAnim;
	m_fAnimationLine = fLineAnim;
}

//*****************************************************************************
//アニメーションの更新処理
//*****************************************************************************
void CScene2D::UpdateAnimation(ANIMATION AnimConditions, TEX texType)
{
	VERTEX_2D *pVtx;

	//カウンターの加算
	m_nCounterAnim++;

	if (m_nCounterAnim == m_fAnimationSpeed)	//アニメーションの速さ
	{
		//カウンターのクリア
		m_nCounterAnim = 0;

		//行数の加算
		m_nPatternAnim++;
	}

	if (m_nPatternAnim == m_fAnimationPattern)	//アニメーションの行数
	{
		//行数のクリア
		m_nPatternAnim = 0;

		//列数の加算
		m_nCounterLine++;
	}

	if (m_nCounterLine == m_fAnimationLine)	//アニメーションの列数
	{
		//列数のクリア
		m_nCounterLine = 0;

		//アニメーションを繰り返さないとき
		if (AnimConditions == ANIMATION_UNREPEAT)
		{
			//終了処理
			Uninit();
		}

	}
	else
	{
		// 頂点データの範囲をロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		switch (texType)
		{
		case NOT_INVERSION:

			//テクスチャ座標の更新
			pVtx[0].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern),									(float)m_nCounterLine * (1.0f / m_fAnimationLine));
			pVtx[1].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern) + (1.0f / m_fAnimationPattern),	(float)m_nCounterLine * (1.0f / m_fAnimationLine));
			pVtx[2].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern),									(float)m_nCounterLine * (1.0f / m_fAnimationLine) + (1.0f / m_fAnimationLine));
			pVtx[3].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern) + (1.0f / m_fAnimationPattern),	(float)m_nCounterLine * (1.0f / m_fAnimationLine) + (1.0f / m_fAnimationLine));

			break;

		case INVERSION:

			//テクスチャ座標の更新
			pVtx[0].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern),									(float)m_nCounterLine * (1.0f / m_fAnimationLine));
			pVtx[1].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern) - (1.0f / m_fAnimationPattern),	(float)m_nCounterLine * (1.0f / m_fAnimationLine));
			pVtx[2].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern),									(float)m_nCounterLine * (1.0f / m_fAnimationLine) + (1.0f / m_fAnimationLine));
			pVtx[3].tex = D3DXVECTOR2((float)m_nPatternAnim * (1.0f / m_fAnimationPattern) - (1.0f / m_fAnimationPattern),	(float)m_nCounterLine * (1.0f / m_fAnimationLine) + (1.0f / m_fAnimationLine));

			break;
		}

		// 頂点データをアンロック
		m_pVtxBuff->Unlock();
	}
}

//*****************************************************************************
//回転処理
//*****************************************************************************
void CScene2D::Rotation(D3DXVECTOR3 pos, D3DXVECTOR3 size, ROTATION rotation)
{
	VERTEX_2D *pVtx;

	float fRollX1;
	float fRollX2;
	float fRollY1;
	float fRollY2;

	//回す角度の加算
	m_fRadian += 0.05f;

	//中心から頂点までの距離を測る（ｒ：中心から頂点までの距離）
	float r = sqrtf((size.x / 2) * (size.x / 2) + (size.y / 2) * (size.y / 2));

	//なす角θを求める（θ：なす角）
	float fTheta = (float)atan2((size.y / 2), (size.x / 2));

	//回転の向き
	switch (rotation)
	{
	case ROTATION_RIGHT:	//右向き

		//増減パターンを作成（Xの増減、Yの増減）
		fRollX1 = cosf(fTheta + m_fRadian) * r;
		fRollX2 = cosf(fTheta - m_fRadian) * r;
		fRollY1 = sinf(fTheta + m_fRadian) * r;
		fRollY2 = sinf(fTheta - m_fRadian) * r;

		break;

	case ROTATION_LEFT:		//左向き

		//増減パターンを作成（Xの増減、Yの増減）
		fRollX1 = cosf(fTheta - m_fRadian) * r;
		fRollX2 = cosf(fTheta + m_fRadian) * r;
		fRollY1 = sinf(fTheta - m_fRadian) * r;
		fRollY2 = sinf(fTheta + m_fRadian) * r;

		break;

	default:

		break;
	}

	//頂点データの範囲をロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//ポリゴン座標の更新
	pVtx[0].pos = D3DXVECTOR3((pos.x - fRollX1), (pos.y - fRollY1), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((pos.x + fRollX2), (pos.y - fRollY2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((pos.x - fRollX2), (pos.y + fRollY2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((pos.x + fRollX1), (pos.y + fRollY1), 0.0f);

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
//サイズの設定
//*****************************************************************************
void CScene2D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//*****************************************************************************
//ポジションの設定
//*****************************************************************************
void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//*****************************************************************************
//カラーの設定
//*****************************************************************************
void CScene2D::SetColor(D3DXCOLOR color)
{
	m_color = color;
}

//*****************************************************************************
//テクスチャの割り当て
//*****************************************************************************
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//*****************************************************************************
//ポジションの取得
//*****************************************************************************
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;
}