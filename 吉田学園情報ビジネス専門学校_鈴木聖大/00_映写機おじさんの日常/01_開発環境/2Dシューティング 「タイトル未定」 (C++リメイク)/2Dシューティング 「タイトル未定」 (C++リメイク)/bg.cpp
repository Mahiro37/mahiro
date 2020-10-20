//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"

//*****************************************************************************
//静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBg::m_apTexture[MAX_BG_TEXTURE] = {};

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CBg::CBg()
{
	memset(m_pScene2D, 0, sizeof(m_pScene2D));
	m_nAnimCounter = 0;
	m_nAnimPattern = 0;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CBg::~CBg()
{
}

//*****************************************************************************
//ロード処理
//*****************************************************************************
HRESULT CBg::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm00.jpg", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm01.jpg", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm02.jpg", &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm03.jpg", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm04.jpg", &m_apTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm05.jpg", &m_apTexture[5]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm06.jpg", &m_apTexture[6]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm07.jpg", &m_apTexture[7]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm08.jpg", &m_apTexture[8]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm09.jpg", &m_apTexture[9]);
	D3DXCreateTextureFromFile(pDevice, "Textures/bg/retrofilm10.jpg", &m_apTexture[10]);

	return S_OK;
}

//*****************************************************************************
//アンロード処理
//*****************************************************************************
void CBg::Unload(void)
{
	//テクスチャの破棄
	for (int nCount = 0; nCount < MAX_BG_TEXTURE; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//背景の生成
//*****************************************************************************
CBg *CBg::Create(void)
{
	//背景のメモリ確保
	CBg *pBg = new CBg;

	//背景の初期化
	pBg->Init();

	//オブジェクトタイプの設定
	pBg->SetObjType(OBJTYPE_BG);

	return pBg;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CBg::Init(void)
{
	for (int nCount = 0; nCount < MAX_BG; nCount++)
	{
		//シーン2Dのメモリ確保
		m_pScene2D[nCount] = new CScene2D;

		if (m_pScene2D[nCount] != NULL)
		{
			//オブジェクトの種類の設定
			m_pScene2D[nCount]->SetObjType(OBJTYPE_BG);

			//サイズの設定
			m_pScene2D[nCount]->SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

			//シーン2Dの初期化処理
			m_pScene2D[nCount]->Init();

			//テクスチャの設定
			m_pScene2D[nCount]->BindTexture(m_apTexture[nCount]);
		}
	}

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CBg::Uninit(void)
{
	//シーン2Dの終了処理
	for (int nCount = 0; nCount < MAX_BG; nCount++)
	{
		if (m_pScene2D[nCount] != NULL)
		{
			m_pScene2D[nCount]->Uninit();
			m_pScene2D[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CBg::Update(void)
{
	m_nAnimCounter++;

	if (m_nAnimCounter == 2)
	{
		m_nAnimCounter = 0;

		for (int nCount = 0; nCount < MAX_BG; nCount++)
		{
			if (m_pScene2D[nCount] != NULL)
			{
				m_nAnimPattern++;

				if (m_nAnimPattern == MAX_BG_TEXTURE)
				{
					m_nAnimPattern = 0;
				}

				//テクスチャの設定
				m_pScene2D[nCount]->BindTexture(m_apTexture[m_nAnimPattern]);
			}
		}
	}
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CBg::Draw(void)
{
}