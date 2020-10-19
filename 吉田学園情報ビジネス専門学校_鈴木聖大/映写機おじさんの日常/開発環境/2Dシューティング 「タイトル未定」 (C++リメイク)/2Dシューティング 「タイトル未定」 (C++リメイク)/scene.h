//=============================================================================
//
// �I�u�W�F�N�g���� [scene.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_OBJECT (256)	//�I�u�W�F�N�g�̍ő吔

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CScene	//�I�u�W�F�N�g�N���X
{
public:
	//�I�u�W�F�N�g�^�C�v�̗񋓑�
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_BG,					//�w�i
		OBJTYPE_DOOR,				//�h�A
		OBJTYPE_UI_BACK,			//��ʂ�UI
		OBJTYPE_EFFECT_BULLET,		//�e�̃G�t�F�N�g
		OBJTYPE_BULLET,				//�o���b�g
		OBJTYPE_FIRST_BALL,			//�t�@�[�X�g�{�[��
		OBJTYPE_SECOND_BALL,		//�Z�J���h�{�[��
		OBJTYPE_THIRD_BALL,			//�T�[�h�{�[��
		OBJTYPE_BOSS_BALL,			//�{�X�{�[��
		OBJTYPE_ITEM,				//�A�C�e��
		OBJTYPE_SCORE,				//�X�R�A
		OBJTYPE_PLAYER,				//�v���C���[
		OBJTYPE_BARRIER,			//�o���A
		OBJTYPE_EFFECT,				//�G�t�F�N�g
		OBJTYPE_EXPLOSION,			//����
		OBJTYPE_UI_FRONT,			//�O�ʂ�UI
		OBJTYPE_UI_ITEM,			//�A�C�e����UI
		OBJTYPE_NUMBER,				//����
		OBJTYPE_PAUSE,				//�|�[�Y
		OBJTYPE_DESCRIPTION,		//�������
		OBJTYPE_START_OR_FINISH,	//�X�^�[�g���̃J�E���g
		OBJTYPE_FADE,				//�t�F�[�h
		OBJTYPE_MAX
	}OBJTYPE;

	CScene();
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static int m_nNumAll;
	static void ReleaseAll(void);
	static void ReleaseAllOtherThanFadeAndPlayer(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	static CScene *m_apScene[MAX_OBJECT];

	static void UpdateStop(bool bUpdateStop);
	void SetObjType(OBJTYPE objType);

	static bool GetUpdateStop(void);
	OBJTYPE GetObjType(void);
	static CScene *GetScene(int nCount);

protected:
	void Release(void);

private:
	int m_nID;
	OBJTYPE m_objType;	//�I�u�W�F�N�g�̎��
	static bool m_bUpdateStop;
};

#endif