//=============================================================================
//
// �v���C���[���� [player.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2d.h"
#include "item.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define PLAYER_TEXTURE		(1)										//�e�N�X�`���̎��
#define PLAYER_LIFE			(3)										//�v���C���[�̎c�@��
#define PLAYER_SIZE			(D3DXVECTOR3(120.0f,150.0f,0.0f))		//�v���C���[�̃T�C�Y
#define PLAYER_RADIUS		(45.0f)									//�v���C���[�̔��a

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CBarrier;
class CBullet;
class CEffect;
class CMessage;

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CPlayer :public CScene2D
{
public:
	//�v���C���[�̏��
	typedef enum
	{
		PLAYER_STATE_NONE = 0,		//�ʏ�
		PLAYER_STATE_INVINCIBLE,	//���G
		PLAYER_STATE_DEATH,			//���S
		PLAYER_STATE_MAX
	}PLAYER_STATE;
	
	CPlayer();
	~CPlayer();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void PlayerReset(void);

	void SetItemStock(CItem::ITEMTYPE ItemType);
	void SetPlayerAbility(void);
	void DeleteBarrierAbility(void);
	void SetLife(int nLife);
	void SetPlayerState(PLAYER_STATE state);
	void SetPressCount(int num);
	void SetBulletPointerNULL(void);

	D3DXVECTOR3 GetPos(void);
	CBarrier *GetBarrier(void);
	CBullet *GetBullet(void);
	int GetPlayerLife(void);
	int GetChargeSpeed(void);
	list<CItem::ITEMTYPE> GetStockItem(void);
	PLAYER_STATE GetPlayerState(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[PLAYER_TEXTURE];
	D3DXVECTOR3 m_pos;
	static int m_nLife;
	int m_nItemScore;
	float m_fPlayerSpeed;
	int m_nChargeSpeed;

	static CBarrier *m_pBarrier;
	CBullet *m_pBullet;
	CEffect *m_pEffect;
	CMessage *m_pMessage;

	list<CItem::ITEMTYPE> m_StockItem;

	PLAYER_STATE m_PlayerState;

	int m_nCntInvincible;
	int m_nInvincibleTime;
	int m_nCntPress;
	int m_nCntCharge;
	int m_nCntItem;
	static bool m_bRelease;
};

#endif