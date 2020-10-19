//================================================================
//
// �w�i���� [bg.h]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _BG_H_
#define _BG_H_

//****************************************************************
//�C���N���[�h�t�@�C��
//****************************************************************
#include "scene.h"
#include "scene2d.h"

//****************************************************************
//�}�N����`
//****************************************************************
#define MAX_BG (1)
#define MAX_BG_TEXTURE (11)		//�e�N�X�`���̎��

//****************************************************************
//�N���X��`
//****************************************************************
class CBg :public CScene
{
public:
	CBg();
	~CBg();
	static HRESULT Load(void);
	static void Unload(void);
	static CBg *Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BG_TEXTURE];
	CScene2D *m_pScene2D[MAX_BG];
	int m_nAnimCounter;					//�A�j���[�V�����̑���
	int m_nAnimPattern;					//�A�j���[�V�����̃p�^�[����
};

#endif