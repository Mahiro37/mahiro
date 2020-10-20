//=============================================================================
//
// 選択処理 [select.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "select.h"
#include "manager.h"
#include "scene.h"
#include "keyboard.h"
#include "bg.h"
#include "player.h"
#include "enemy.h"
#include "fade.h"
#include "door.h"
#include "ui.h"
#include "sound.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CSelect::CSelect()
{
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CSelect::~CSelect()
{
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CSelect::Init(void)
{
	//BGMの再生
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM01);

	//背景の生成
	CBg::Create();

	//プレイヤーの生成
	CManager::CreatePlayer();

	//ドアの生成
	CDoor::Create(D3DXVECTOR3(229.8f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_GAME);
	CDoor::Create(D3DXVECTOR3(434.6f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_SCORE_ATTACK);
	CDoor::Create(D3DXVECTOR3(639.4f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_TUTORIAL_GAME);
	CDoor::Create(D3DXVECTOR3(844.2f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_RANKING);
	CDoor::Create(D3DXVECTOR3(1049.0f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_TITLE);


	//セレクト文字の生成
	CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 160.0f, 0.0f), D3DXVECTOR3(600.0f, 200.0f, 0.0f), CUi::UI_TYPE_SELECT);

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CSelect::Uninit(void)
{
	//BGMの停止
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM01);

	//全てのメモリの開放処理
	CScene::ReleaseAllOtherThanFadeAndPlayer();
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CSelect::Update(void)
{
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CSelect::Draw(void)
{
}