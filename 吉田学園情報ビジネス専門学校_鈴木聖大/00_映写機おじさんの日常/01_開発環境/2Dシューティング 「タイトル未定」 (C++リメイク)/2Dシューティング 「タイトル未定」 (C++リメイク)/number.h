//=============================================================================
//
// �����̏��� [number.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
//�w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2d.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define NUMBER_SIZE_X		(40.0f)		//������X�T�C�Y
#define NUMBER_SIZE_Y		(50.0f)		//������Y�T�C�Y

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CNumber
{
public:
	//�����̎�ނ̗񋓌^
	typedef enum
	{
		TYPE_1 = 0,
		TYPE_2,
		TYPE_MAX
	}TYPE;

	CNumber();
	~CNumber();
	static HRESULT Load(void);
	static void Unload(void);
	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumber, TYPE type);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumber, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);
	void SetSize(D3DXVECTOR3 size);
	void SetPos(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPos(void);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];
	static LPDIRECT3DDEVICE9 m_pDevice;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	int m_nNumber;
	TYPE m_type;
};

#endif