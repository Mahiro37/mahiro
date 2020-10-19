//================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//****************************************************************
//�C���N���[�h�t�@�C��
//****************************************************************
#include "scene2d.h"
#include "sound.h"

//****************************************************************
//�N���X��`
//****************************************************************
class CEffect :public CScene2D
{
public:
	//�G�t�F�N�g�̃^�C�v
	typedef enum
	{
		EFFECT_TYPE_NONE = 0,
		EFFECT_TYPE_GET_SCORE,			//�X�R�A���Z�̃A�C�e���擾
		EFFECT_TYPE_GET_SPEED_UP,		//�X�s�[�h�A�b�v�̃A�C�e���擾
		EFFECT_TYPE_GET_TWIN_BULLET,	//�c�C���e�̃A�C�e���擾
		EFFECT_TYPE_GET_BARRIER,		//�o���A�̃A�C�e���擾
		EFFECT_TYPE_BARRIER,			//�o���A
		EFFECT_TYPE_CHARGE_SHOT,		//�`���[�W�V���b�g
		EFFECT_TYPE_MAX
	}EFFECT_TYPE;

	CEffect();
	~CEffect();
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fEffectSpeed, EFFECT_TYPE EffectType, ANIMATION Animation,CSound::SOUND_LABEL SoundLabel);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fEffectSpeed, EFFECT_TYPE EffectType, ANIMATION Animation);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetAnimation(ANIMATION Animation);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[EFFECT_TYPE_MAX];
	EFFECT_TYPE m_EffectType;
	ANIMATION m_Animation;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_SetSize;
	D3DXCOLOR m_color;
	int m_nLifeCounter;
};

#endif