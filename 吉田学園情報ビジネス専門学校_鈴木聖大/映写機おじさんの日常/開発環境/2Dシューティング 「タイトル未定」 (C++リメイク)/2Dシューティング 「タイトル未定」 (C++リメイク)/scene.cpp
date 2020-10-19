//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
int CScene::m_nNumAll = 0;
CScene *CScene::m_apScene[MAX_OBJECT] = {};
bool CScene::m_bUpdateStop = false;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CScene::CScene()
{
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (m_apScene[nCount] == NULL)
		{
			m_apScene[nCount] = this;	//this : newしたポインタ
			m_nID = nCount;				//何番目か記録
			m_nNumAll++;				//総数をカウント

			break;
		}
	}
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CScene::~CScene()
{
}

//*****************************************************************************
//全てのメモリの開放処理
//*****************************************************************************
void CScene::ReleaseAll(void)
{
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (m_apScene[nCount] != NULL)
		{
			//メモリが確保されていたら処理をする
			m_apScene[nCount]->Uninit();
		}
	}
}

//*****************************************************************************
//フェードとプレイヤー以外のメモリの開放処理
//*****************************************************************************
void CScene::ReleaseAllOtherThanFadeAndPlayer(void)
{
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (m_apScene[nCount] != NULL)
		{
			if (m_apScene[nCount]->GetObjType() != OBJTYPE_FADE)
			{
				//メモリが確保されていたら処理をする
				m_apScene[nCount]->Uninit();
			}
		}
	}
}

//*****************************************************************************
//全ての更新処理
//*****************************************************************************
void CScene::UpdateAll(void)
{
	if (m_bUpdateStop == false)
	{
		for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
		{
			if (m_apScene[nCount] != NULL)
			{
				//メモリが確保されていたら処理をする
				m_apScene[nCount]->Update();
			}
		}
	}
	else if (m_bUpdateStop == true)
	{
		for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
		{
			if (m_apScene[nCount] != NULL)
			{
				if (m_apScene[nCount]->GetObjType() == OBJTYPE_START_OR_FINISH || m_apScene[nCount]->GetObjType() == OBJTYPE_FADE || m_apScene[nCount]->GetObjType() == OBJTYPE_BG || m_apScene[nCount]->GetObjType() == OBJTYPE_PAUSE)
				{
					//メモリが確保されていたら処理をする
					m_apScene[nCount]->Update();
				}
			}
		}
	}
}

//*****************************************************************************
//全ての描画処理
//*****************************************************************************
void CScene::DrawAll(void)
{
	for (int nCountType = 0; nCountType < OBJTYPE_MAX; nCountType++)
	{
		for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
		{
			if (m_apScene[nCount] != NULL && m_apScene[nCount]->GetObjType() == nCountType)
			{
				//メモリが確保されていたら処理をする
				m_apScene[nCount]->Draw();
			}
		}
	}
}

//*****************************************************************************
//メモリの開放処理
//*****************************************************************************
void CScene::Release(void)
{
	if (m_apScene[m_nID] != NULL)
	{
		int nID = m_nID;	//deleteしたらm_IDの情報が消えるので変数に入れる

		//m_apSceneメモリを開放
		delete m_apScene[nID];
		m_apScene[nID] = NULL;

		m_nNumAll--;
	}
}

//*****************************************************************************
//更新を一時的に止める処理
//*****************************************************************************
void CScene::UpdateStop(bool bUpdateStop)
{
	m_bUpdateStop = bUpdateStop;
}

//*****************************************************************************
//オブジェクトタイプの設定
//*****************************************************************************
void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

//*****************************************************************************
//更新が止まっているかを取得
//*****************************************************************************
bool CScene::GetUpdateStop(void)
{
	return m_bUpdateStop;
}

//*****************************************************************************
//オブジェクトタイプの取得
//*****************************************************************************
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}

//*****************************************************************************
//シーンの取得
//*****************************************************************************
CScene *CScene::GetScene(int nCount)
{
	return m_apScene[nCount];
}