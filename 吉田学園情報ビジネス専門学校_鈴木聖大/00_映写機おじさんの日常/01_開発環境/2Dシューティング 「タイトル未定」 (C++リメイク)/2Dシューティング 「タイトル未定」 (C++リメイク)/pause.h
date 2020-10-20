//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
//�w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CUi;

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CPause :public CScene
{
public:
	//�|�[�Y��ʂ̍��ڂ̗񋓌^
	typedef enum
	{
		MENU_CONTINUE = 0,		//������
		MENU_INSTRUCTION,		//�������
		MENU_START_OVER,		//�ŏ������蒼��
		MENU_RETURN_TO_SELECT,	//�Z���N�g�ɖ߂�
		MENU_MAX			
	}MENU;

	CPause();
	~CPause();
	static CPause *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CUi *m_apMenuUi[MENU_MAX];
	CUi *m_pUi;
	CUi *m_pInstructionUi;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	int m_nChoiceNum;
	bool m_bStickMove;
};

#endif