//=============================================================================
//
// UI���� [ui.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
//�w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2d.h"

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CUi:public CScene2D
{
public:
	//UI�̎�ނ̗񋓌^
	typedef enum
	{
		UI_TYPE_NONE = 0,
		UI_TYPE_TIMER,					//�^�C�}�[
		UI_TYPE_ZANKI,					//�c�@
		UI_TYPE_FILM,					//�t�B����
		UI_TYPE_SCORE,					//�X�R�A
		UI_TYPE_PLAYER_SPEED_UP,		//�v���C���[�̈ړ��X�s�[�h�A�b�v
		UI_TYPE_BULLET_SPEED_UP,		//�v���C���[�̒e�̃X�s�[�h�A�b�v
		UI_TYPE_BARRIER,				//�o���A
		UI_TYPE_RANKING_1,				//1��
		UI_TYPE_RANKING_2,				//2��
		UI_TYPE_RANKING_3,				//3��
		UI_TYPE_RANKING_4,				//4��
		UI_TYPE_RANKING_5,				//5��
		UI_TYPE_RANKING_6,				//6��
		UI_TYPE_RANKING_7,				//7��
		UI_TYPE_RANKING_8,				//8��
		UI_TYPE_RANKING_9,				//9��
		UI_TYPE_RANKING_10,				//10��
		UI_TYPE_START,					//�X�^�[�g
		UI_TYPE_FINISH,					//�t�B�j�b�V��
		UI_TYPE_GAME_OVER,				//�Q�[���I�[�o�[
		UI_TYPE_GAME_CLEAR,				//�Q�[���N���A
		UI_TYPE_STAGE_1,				//�X�e�[�W�P
		UI_TYPE_STAGE_2,				//�X�e�[�W�Q
		UI_TYPE_STAGE_3,				//�X�e�[�W�R
		UI_TYPE_GAME_SCORE_SCREEN,		//�X�R�A�\�����
		UI_TYPE_RANK_IN,				//�����N�C��
		UI_TYPE_SELECT,					//�Z���N�g
		UI_TYPE_SELECT_TITLE,			//�^�C�g�����S(�h�A�p)
		UI_TYPE_SELECT_GAME,			//�Q�[�����S(�h�A�p)
		UI_TYPE_SELECT_SCORE_ATTACK,	//�X�R�A�A�^�b�N���S(�h�A�p)
		UI_TYPE_SELECT_TUTORIAL,		//�`���[�g���A�����S(�h�A�p)
		UI_TYPE_SELECT_RANKING,			//�����L���O���S(�h�A�p)
		UI_TYPE_SELECT_SELECT,			//�Z���N�g���S(�h�A�p)
		UI_TYPE_BALL_CREATE,			//�{�[���̐����ꏊ
		UI_TYPE_TUTORIAL_GAME,			//�`���[�g���A���̃Q�[�����[�h�\��
		UI_TYPE_TUTORIAL_SCORE_ATTACK,	//�`���[�g���A���̃X�R�A�A�^�b�N���[�h�\��
		UI_TYPE_FUKIDASHI,				//�����o��
		UI_TYPE_RANKING_MESSAGE,		//�����L���O�����p�̃��b�Z�[�W
		UI_TYPE_DESCRIPTION,			//�������
		UI_TYPE_TITLE,					//�Q�[���^�C�g��
		UI_TYPE_CONTINUE,				//������(�|�[�Y)
		UI_TYPE_INSTRUCTION,			//�������(�|�[�Y)
		UI_TYPE_START_OVER,				//�ŏ�����(�|�[�Y)
		UI_TYPE_RETURN_TO_SELECT,		//�Z���N�g��(�|�[�Y)
		UI_TYPE_PAUSE,					//�|�[�Y�w�i
		UI_TYPE_MAX
	}UI_TYPE;

	CUi();
	~CUi();
	static HRESULT Load(void);
	static void Unload(void);
	static CUi *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI_TYPE type);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPauseChoice(bool bPauseChoice);
	void BindTexture(UI_TYPE type);

	bool GetPauseChoice(void);
	UI_TYPE GetUiType(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[UI_TYPE_MAX];
	UI_TYPE m_type;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_move;
	D3DXCOLOR m_color;
	int m_nCntColor;
	int m_nCntUi;
	bool m_bLogoMove;
	bool m_bTransparence;
	bool m_bPauseChoice;
};

#endif