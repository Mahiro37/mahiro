//=============================================================================
//
// ドア処理 [door.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "door.h"
#include "renderer.h"
#include "keyboard.h"
#include "controller.h"
#include "player.h"
#include "fade.h"
#include "ui.h"
#include "sound.h"

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CDoor::m_pTexture = NULL;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CDoor::CDoor()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mode = CManager::MODE_NONE;
	m_nCntLit = 0;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CDoor::~CDoor()
{
}

//*****************************************************************************
//ロード処理
//*****************************************************************************
HRESULT CDoor::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "Textures/ui/door.png", &m_pTexture);

	return S_OK;
}

//*****************************************************************************
//アンロード処理
//*****************************************************************************
void CDoor::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//*****************************************************************************
//数字の生成
//*****************************************************************************
CDoor *CDoor::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE mode)
{
	//数字のメモリ確保
	CDoor *pDoor = new CDoor;

	if (pDoor != NULL)
	{
		//数字の初期化処理
		pDoor->Init(pos, size, mode);
	}

	return pDoor;
}

HRESULT CDoor::Init(void)
{
	return S_OK;
}

//*****************************************************************************
//初期化処理
//*****************************************************************************
HRESULT CDoor::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE mode)
{
	//オブジェクトの種類の設定
	SetObjType(OBJTYPE_DOOR);

	//タイプの設定
	BindTexture(m_pTexture);

	//モードの設定
	m_mode = mode;

	//位置の設定
	m_pos = pos;
	SetPos(pos);

	//サイズの設定
	SetSize(size);

	//シーン2Dの初期化
	CScene2D::Init();

	//ドア用ロゴUIの生成
	switch (mode)
	{
	case CManager::MODE_TITLE:

		CUi::Create(D3DXVECTOR3(pos.x, pos.y - 160.0f, pos.z), D3DXVECTOR3(150.0f, 70.0f, 0.0f), CUi::UI_TYPE_SELECT_TITLE);

		break;

	case CManager::MODE_SELECT:

		CUi::Create(D3DXVECTOR3(pos.x, pos.y - 160.0f, pos.z), D3DXVECTOR3(150.0f, 70.0f, 0.0f), CUi::UI_TYPE_SELECT_SELECT);

		break;

	case CManager::MODE_GAME:

		CUi::Create(D3DXVECTOR3(pos.x, pos.y - 160.0f, pos.z), D3DXVECTOR3(150.0f, 70.0f, 0.0f), CUi::UI_TYPE_SELECT_GAME);

		break;

	case CManager::MODE_SCORE_ATTACK:

		CUi::Create(D3DXVECTOR3(pos.x, pos.y - 165.0f, pos.z), D3DXVECTOR3(160.0f, 80.0f, 0.0f), CUi::UI_TYPE_SELECT_SCORE_ATTACK);

		break;

	case CManager::MODE_TUTORIAL_GAME:

		CUi::Create(D3DXVECTOR3(pos.x, pos.y - 165.0f, pos.z), D3DXVECTOR3(160.0f, 80.0f, 0.0f), CUi::UI_TYPE_SELECT_TUTORIAL);

		break;

	case CManager::MODE_RANKING:

		CUi::Create(D3DXVECTOR3(pos.x, pos.y - 160.0f, pos.z), D3DXVECTOR3(150.0f, 70.0f, 0.0f), CUi::UI_TYPE_SELECT_RANKING);

		break;

	default:

		break;
	}

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************
void CDoor::Uninit(void)
{
	//シーン2Dの終了処理
	CScene2D::Uninit();

	m_mode = CManager::MODE_NONE;
}

//*****************************************************************************
//更新処理
//*****************************************************************************
void CDoor::Update(void)
{
	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		//オブジェクトの取得
		CScene *pScene = GetScene(nCntScene);

		//ダイナミックキャスト
		CPlayer *pPlayer = dynamic_cast<CPlayer*>(pScene);

		if (pPlayer != NULL)
		{
			D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

			if (PlayerPos.x < m_pos.x + (DOOR_SIZE.x / 2) &&
				PlayerPos.x > m_pos.x - (DOOR_SIZE.x / 2) &&
				PlayerPos.y < m_pos.y + (DOOR_SIZE.y / 2) &&
				PlayerPos.y > m_pos.y - (DOOR_SIZE.y / 2))
			{
				//点灯カウントを進める
				m_nCntLit++;

				if (m_nCntLit < 30)
				{ //30フレームより小さいとき
					//カラーの更新
					SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
				}
				else if (m_nCntLit >= 30 && m_nCntLit < 60)
				{ //30フレーム以上のとき
					//カラーの更新
					SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else if (m_nCntLit == 60)
				{ //60フレーム経ったとき
					//点灯カウントの初期化
					m_nCntLit = 0;
				}

				CKeyboard *pKeyboard = CManager::GetKeyboard();

				if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || CManager::GetController()->GetJoypadTrigger(3))
				{ //Enterキー または Bボタンを押したとき

					//SEの再生
					CManager::GetSound()->Play(CSound::SOUND_LABEL_DOOR_OPEN);

					switch (m_mode)
					{
					case CManager::MODE_TITLE:

						//フェードの生成
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_TITLE);

						break;

					case CManager::MODE_SELECT:

						//フェードの生成
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_SELECT);

						break;

					case CManager::MODE_GAME:

						//フェードの生成
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_GAME);

						break;

					case CManager::MODE_SCORE_ATTACK:

						//フェードの生成
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_SCORE_ATTACK);

						break;

					case CManager::MODE_TUTORIAL:

						//フェードの生成
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_TUTORIAL);

						break;

					case CManager::MODE_TUTORIAL_GAME:

						//フェードの生成
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_TUTORIAL_GAME);

						break;

					case CManager::MODE_TUTORIAL_SCORE_ATTACK:

						//フェードの生成
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_TUTORIAL_SCORE_ATTACK);

						break;

					case CManager::MODE_RANKING:

						//フェードの生成
						CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_RANKING);

						break;

					default:

						break;
					}
				}
			}
			else
			{
				//カラーの更新
				SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				//点灯カウントの初期化
				m_nCntLit = 0;
			}

			break;
		}
	}

	//シーン2Dの更新
	CScene2D::Update();
}

//*****************************************************************************
//描画処理
//*****************************************************************************
void CDoor::Draw(void)
{
	//シーン2Dの描画処理
	CScene2D::Draw();
}