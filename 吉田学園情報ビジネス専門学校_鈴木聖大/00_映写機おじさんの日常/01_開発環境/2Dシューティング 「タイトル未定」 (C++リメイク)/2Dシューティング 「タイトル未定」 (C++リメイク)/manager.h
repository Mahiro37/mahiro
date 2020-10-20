//=============================================================================
//
// �}�l�W�����g���� [manager.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "number.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CRenderer;
class CKeyboard;
class CController;
class CSound;
class CMode;
class CTime;
class CLife;
class CPlayer;

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CManager
{
public:
	//��ʑJ�ڂ̗񋓌^
	typedef enum
	{
		MODE_NONE = 0,
		MODE_TITLE,					//�^�C�g��
		MODE_SELECT,				//�Z���N�g
		MODE_GAME,					//�Q�[��
		MODE_GAME_CREAR,			//�Q�[���N���A���
		MODE_SCORE_ATTACK,			//�X�R�A�A�^�b�N
		MODE_SCORE_SCREEN,			//�X�R�A�\�����
		MODE_TUTORIAL,				//�`���[�g���A��
		MODE_TUTORIAL_GAME,			//�`���[�g���A��(�Q�[�����[�h)
		MODE_TUTORIAL_SCORE_ATTACK,	//�`���[�g���A��(�X�R�A�A�^�b�N���[�h)
		MODE_RANKING,				//�����L���O
		MODE_MAX
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void CreateRenderer(HWND hWnd);
	static HRESULT CreateKeyboard(HINSTANCE hInstance, HWND hWnd);
	static HRESULT CreateController(HINSTANCE hInstance, HWND hWnd);
	static void CreateSound(HWND hWnd);
	static void CreateTime(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, CNumber::TYPE type);
	static void CreateLife(D3DXVECTOR3 pos);
	static void CreatePlayer(void);

	static CRenderer *GetRenderer(void);
	static CKeyboard *GetKeyboard(void);
	static CController *GetController(void);
	static CSound *GetSound(void);
	static CLife *GetLife(void);
	static CTime *GetTime(void);
	static CPlayer *GetPlayer(void);
	static MODE GetMode(void);
	static CMode *GetModePointer(void);

	void SetMode(MODE mode);

private:
	static CRenderer *m_pRenderer;
	static CKeyboard *m_pKeyboard;
	static CController *m_pController;
	static CSound *m_pSound;
	static CMode *m_pMode;
	static CLife *m_pLife;
	static CTime *m_pTime;
	static CPlayer *m_pPlayer;
	static MODE m_mode;
};

#endif