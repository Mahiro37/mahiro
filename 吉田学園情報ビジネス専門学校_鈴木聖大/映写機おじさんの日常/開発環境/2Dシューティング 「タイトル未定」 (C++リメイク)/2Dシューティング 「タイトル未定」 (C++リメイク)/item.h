//================================================================
//
// �A�C�e������ [item.cpp]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//****************************************************************
//�C���N���[�h�t�@�C��
//****************************************************************
#include "scene2d.h"
#include <list>
using namespace std;

//****************************************************************
//�}�N����`
//****************************************************************
#define MAX_ITEM				(256)							//�A�C�e���̍ő吔
#define MAX_ITEM_TEXTURE		(32)							//�e�N�X�`���̎��
#define ITEM_SIZE				(D3DXVECTOR3(50.0f,50.0f,0.0f))	//�A�C�e���̃T�C�Y
#define ITEM_RADIUS				(25.0f)							//�A�C�e���̔��a
#define ITEM_DIVISION_POS_X		(0.0f)							//�����̏���X���W
#define ITEM_DIVISION_POS_Y		(-20.0f)						//�����̏���Y���W

//****************************************************************
//�N���X��`
//****************************************************************
class CItem :public CScene2D
{
public:
	//�A�C�e���^�C�v�̗񋓑�
	typedef enum
	{
		ITEMTYPE_NONE = 0,
		ITEMTYPE_PLAYER_SPEED_UP,	//�X�s�[�h�A�b�v
		ITEMTYPE_CHARGE_SPEED_UP,	//�`���[�W�X�s�[�h�A�b�v
		ITEMTYPE_BARRIER,			//�o���A
		ITEMTYPE_RESET,				//�A�C�e�����Z�b�g
		ITEMTYPE_TIME_EXTENSION_1,	//���ԉ���(+1)
		ITEMTYPE_TIME_EXTENSION_2	//���ԉ���(+5)
	}ITEMTYPE;
	
	CItem();
	~CItem();
	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, OBJTYPE type);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move);

	ITEMTYPE GetItemType(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_ITEM_TEXTURE];
	static CItem *m_pItem[MAX_ITEM];
	LPDIRECT3DTEXTURE9 m_apScene[MAX_ITEM_TEXTURE];
	D3DXVECTOR3 m_move;
	int m_nItemCounter;
	ITEMTYPE m_ItemType;
};

#endif