//=============================================================================
//
// 数字の処理 [number.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "number.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define NUM_VERTEX	(4)		//頂点数
#define NUM_POLYGON (2)		//ポリゴン (三角形の数)
#define NUMBER_NUM	(10)	//数字の数

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 CNumber::m_pDevice = NULL;
LPDIRECT3DTEXTURE9 CNumber::m_pTexture[TYPE_MAX] = {};

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CNumber::CNumber()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNumber = 0;
	m_type = TYPE_1;
	m_pVtxBuff = NULL;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CNumber::~CNumber()
{
}

//*****************************************************************************
//ロード処理
//*****************************************************************************
HRESULT CNumber::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "Textures/number/number00.png", &m_pTexture[TYPE_1]);
	D3DXCreateTextureFromFile(pDevice, "Textures/number/number01.png", &m_pTexture[TYPE_2]);

	return S_OK;
}

//*****************************************************************************
//アンロード処理
//*****************************************************************************
void CNumber::Unload(void)
{
	// テクスチャの破棄
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//数字の生成
//*****************************************************************************
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumber, TYPE type)
{
	//数字のメモリ確保
	CNumber *pNumber = new CNumber;

	if (pNumber != NULL)
	{
		//数字の初期化処理
		pNumber->Init(pos, size, nNumber, type);
	}

	return pNumber;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumber, TYPE type)
{
	m_pos = pos;
	m_size = size;
	m_nNumber = nNumber;
	m_type = type;

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber + (1.0f / NUMBER_NUM), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber + (1.0f / NUMBER_NUM), 1.0f);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CNumber::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CNumber::Update(void)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの位置を設定
	pVtx[0].pos = D3DXVECTOR3((m_pos.x - (m_size.x / 2)), (m_pos.y - (m_size.y / 2)), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((m_pos.x + (m_size.x / 2)), (m_pos.y - (m_size.y / 2)), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((m_pos.x - (m_size.x / 2)), (m_pos.y + (m_size.y / 2)), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((m_pos.x + (m_size.x / 2)), (m_pos.y + (m_size.y / 2)), 0.0f);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CNumber::Draw(void)
{
	// 頂点バッファをデータストリームに設定
	m_pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	m_pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	m_pDevice->SetTexture(0, m_pTexture[m_type]);

	// ポリゴンの描画
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//*****************************************************************************
//設定処理
//*****************************************************************************
void CNumber::SetNumber(int nNumber)
{
	m_nNumber = nNumber;

	VERTEX_2D *pVtx;

	// 頂点データの範囲をロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber + (1.0f / NUMBER_NUM), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)m_nNumber + (1.0f / NUMBER_NUM), 1.0f);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
//サイズの設定
//*****************************************************************************
void CNumber::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//*****************************************************************************
//ポジションの設定
//*****************************************************************************
void CNumber::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//*****************************************************************************
//ポジションの取得
//*****************************************************************************
D3DXVECTOR3 CNumber::GetPos(void)
{
	return m_pos;
}