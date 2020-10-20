//=============================================================================
//
// �Q�[���N���A���� [GameClear.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
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
//�}�N����`
//*****************************************************************************
#define SELECT_TIME	(1800)	//�Z���N�g��ʂɑJ�ڂ��鎞��

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CGameClear::CGameClear()
{
	m_nSelectCounter = 0;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CGameClear::~CGameClear()
{
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CGameClear::Init(void)
{
	//BGM�̍Đ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM02);

	//�w�i�̐���
	CBg::Create();

	//�Q�[���N���AUI�̐���
	CUi::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(850.0f, 300.0f, 0.0f), CUi::UI_TYPE_GAME_CLEAR);

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CGameClear::Uninit(void)
{
	//BGM�̒�~
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_BGM02);

	//�S�Ẵ������̊J������
	CScene::ReleaseAllOtherThanFadeAndPlayer();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CGameClear::Update(void)
{
	if (m_nSelectCounter <= SELECT_TIME)
	{
		m_nSelectCounter++;
	}

	CKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || CManager::GetController()->GetJoypadTrigger(3) || m_nSelectCounter == SELECT_TIME)
	{ //Enter�L�[���������Ƃ� �܂��� B�{�^�����������Ƃ� �܂��� 10�b�o�����Ƃ�
		//�t�F�[�h�̐���
		CFade::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CManager::MODE_SELECT);
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CGameClear::Draw(void)
{
}