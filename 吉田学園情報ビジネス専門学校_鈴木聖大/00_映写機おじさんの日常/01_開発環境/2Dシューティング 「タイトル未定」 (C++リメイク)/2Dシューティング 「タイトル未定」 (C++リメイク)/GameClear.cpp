//=============================================================================
//
// ゲームクリア処理 [GameClear.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "GameClear.h"
#include "manager.h"
#include "keyboard.h"
#include "controller.h"
#include "bg.h"
#include "fade.h"
#include "ui.h"
#include "sound.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define SELECT_TIME	(1800)	//セレクト画面に遷移する時間

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CGameClear::CGameClear()
{
	m_nSelectCounter = 0;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CGameClear::~CGameClear()
{
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CGameClear::Init(void)
{
	//BGMの再生
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM02);

	//背景の生成
	CBg::Create();

	//ゲームクリアUIの生成
	CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(850.0f, 300.0f, 0.0f), CUi::UI_TYPE_GAME_CLEAR);

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CGameClear::Uninit(void)
{
	//BGMの停止
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM02);

	//全てのメモリの開放処理
	CScene::ReleaseAllOtherThanFadeAndPlayer();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CGameClear::Update(void)
{
	if (m_nSelectCounter <= SELECT_TIME)
	{
		m_nSelectCounter++;
	}

	CKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || CManager::GetController()->GetJoypadTrigger(3) || m_nSelectCounter == SELECT_TIME)
	{ //Enterキーを押したとき または Bボタンを押したとき または 10秒経ったとき
		//フェードの生成
		CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_SELECT);
	}
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CGameClear::Draw(void)
{
}