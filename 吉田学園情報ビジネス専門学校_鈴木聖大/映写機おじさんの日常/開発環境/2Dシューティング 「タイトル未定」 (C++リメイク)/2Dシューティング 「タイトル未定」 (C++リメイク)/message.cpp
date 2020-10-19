//=============================================================================
//
// メッセージ表示処理 [message.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "message.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define COUNT_TIME		(60)	//表示する時間

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
int CMessage::m_nCounter = 0;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CMessage::CMessage()
{
	m_pUi = NULL;
	m_pFade = NULL;
	m_mode = CManager::MODE_NONE;
	m_nTime = 0;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CMessage::~CMessage()
{
}

//*****************************************************************************
//スタートカウントの生成
//*****************************************************************************
CMessage *CMessage::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, CUi::UI_TYPE type, CManager::MODE mode)
{
	//メッセージのメモリ確保
	CMessage *pMessage = new CMessage;

	//メッセージの初期化処理
	if (pMessage != NULL)
	{
		pMessage->Init(pos, size, nTime, type, mode);
	}

	return pMessage;
}

HRESULT CMessage::Init(void)
{
	return S_OK;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CMessage::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, CUi::UI_TYPE type, CManager::MODE mode)
{
	switch (type)
	{
	case CUi::UI_TYPE_GAME_OVER:

		//SEの再生
		CManager::GetSound()->Play(CSound::SOUND_LABEL_GAME_OVER);

		break;

	default:

		break;
	}

	//UIの生成
	if (m_pUi == NULL)
	{
		m_pUi = CUi::Create(pos, size, type);
	}

	if (m_pUi != NULL)
	{
		//時間の設定
		m_nTime = nTime;

		switch (CManager::GetMode())
		{
		case CManager::MODE_SCORE_ATTACK:

			//モードの設定
			m_mode = CManager::MODE_SCORE_SCREEN;

			break;

		default:

			//モードの設定
			m_mode = mode;

			break;
		}

		//オブジェクトの種類の設定
		SetObjType(OBJTYPE_START_OR_FINISH);

		//シーンの更新を止める
		UpdateStop(true);
	}

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CMessage::Uninit(void)
{
	m_nCounter = 0;

	if (m_pUi != NULL)
	{
		m_pUi->Uninit();
		m_pUi = NULL;
	}

	if (m_pFade != NULL)
	{
		m_pFade = NULL;
	}

	//メモリの開放処理
	Release();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CMessage::Update(void)
{
	m_nCounter++;

	if (m_nCounter == m_nTime)
	{ //指定の時間経ったとき
		//フェードの生成
		if (m_pFade == NULL)
		{
			m_pFade = CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), m_mode);
		}
	}

	if (m_pFade != NULL)
	{
		if (m_pFade->GetWrapBack() == true)
		{ //フェードが折り返したとき
			//終了処理
			Uninit();
		}
	}
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CMessage::Draw(void)
{
}