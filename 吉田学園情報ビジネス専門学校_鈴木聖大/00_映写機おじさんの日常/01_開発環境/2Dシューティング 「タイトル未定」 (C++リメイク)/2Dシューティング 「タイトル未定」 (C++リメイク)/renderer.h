//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();							//�R���X�g���N�^
	~CRenderer();							//�f�X�g���N�^
	HRESULT Init(HWND hWnd, bool bWindow);	//����������
	void Uninit(void);						//�I������
	void Update(void);						//�X�V����
	void Draw();							//�`�揈��
	LPDIRECT3DDEVICE9 GetDevice(void);

private:
	void DrawFPS(int nCountFPS);

	LPDIRECT3D9 m_pD3D;
	LPD3DXFONT m_pFont;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
};

#endif