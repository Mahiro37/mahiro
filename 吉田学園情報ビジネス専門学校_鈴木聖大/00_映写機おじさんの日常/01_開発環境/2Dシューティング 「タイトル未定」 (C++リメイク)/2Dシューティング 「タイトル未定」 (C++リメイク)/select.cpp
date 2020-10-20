//=============================================================================
//
// �I������ [select.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
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
//�R���X�g���N�^
//*****************************************************************************
CSelect::CSelect()
{
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CSelect::~CSelect()
{
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CSelect::Init(void)
{
	//BGM�̍Đ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM01);

	//�w�i�̐���
	CBg::Create();

	//�v���C���[�̐���
	CManager::CreatePlayer();

	//�h�A�̐���
	CDoor::Create(D3DXVECTOR3(229.8f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_GAME);
	CDoor::Create(D3DXVECTOR3(434.6f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_SCORE_ATTACK);
	CDoor::Create(D3DXVECTOR3(639.4f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_TUTORIAL_GAME);
	CDoor::Create(D3DXVECTOR3(844.2f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_RANKING);
	CDoor::Create(D3DXVECTOR3(1049.0f, 520.0f, 0.0f), DOOR_SIZE, CManager::MODE_TITLE);


	//�Z���N�g�����̐���
	CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 160.0f, 0.0f), D3DXVECTOR3(600.0f, 200.0f, 0.0f), CUi::UI_TYPE_SELECT);

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CSelect::Uninit(void)
{
	//BGM�̒�~
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM01);

	//�S�Ẵ������̊J������
	CScene::ReleaseAllOtherThanFadeAndPlayer();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CSelect::Update(void)
{
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CSelect::Draw(void)
{
}