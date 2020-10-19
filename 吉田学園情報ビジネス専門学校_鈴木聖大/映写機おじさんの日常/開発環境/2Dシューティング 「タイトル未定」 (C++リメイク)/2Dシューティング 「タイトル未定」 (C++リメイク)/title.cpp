//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "title.h"
#include "manager.h"
#include "keyboard.h"
#include "controller.h"
#include "bg.h"
#include "enemy.h"
#include "fade.h"
#include "message.h"
#include "score.h"
#include "time.h"
#include "sound.h"

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CTitle::CTitle()
{
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CTitle::~CTitle()
{
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CTitle::Init(void)
{
	//BGM�̍Đ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM00);

	//�w�i�̐���
	CBg::Create();

	//�Q�[���^�C�g����UI�̐���
	CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(800.0f, 420.0f, 0.0f), CUi::UI_TYPE_TITLE);

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CTitle::Uninit(void)
{
	//BGM�̒�~
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM00);

	//�S�Ẵ������̊J������
	CScene::ReleaseAllOtherThanFadeAndPlayer();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CTitle::Update(void)
{
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || CManager::GetController()->GetJoypadTrigger(3))
	{ //Enter�L�[ �܂��� B�{�^�����������Ƃ�
		//�t�F�[�h�̐���
		CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_SELECT);
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CTitle::Draw(void)
{
}