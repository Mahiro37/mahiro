//=============================================================================
//
// �`���[�g���A������ [tutorial.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
//�w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "mode.h"

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CTutorial :public CMode
{
public:
	//�`���[�g���A���̃��[�h�̗񋓌^
	typedef enum
	{
		TUTORIAL_MODE_NONE = 0,
		TUTORIAL_MODE_GAME,			//�X�e�[�W���[�h
		TUTORIAL_MODE_SCORE_ATTACK,	//�X�R�A�A�^�b�N���[�h
		TUTORIAL_MODE_MAX
	}TUTORIAL_MODE;

	CTutorial();
	~CTutorial();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTutorialMode(TUTORIAL_MODE TutorialMode);

	static TUTORIAL_MODE GetTutorialMode(void);

private:
	static TUTORIAL_MODE m_TutorialMode;
};

#endif