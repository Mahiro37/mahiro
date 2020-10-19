//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "effect.h"
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "player.h"
#include "bullet.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define EFFECT_SIZE			(50)	//�G�t�F�N�g�̑傫��
#define ANIMATION_SPEED		(5)		//�A�j���[�V�����̑���
#define ANIMATION_PATTERN	(8)		//�A�j���[�V�����̃p�^�[����
#define SHOT_EFFECT_LIFE	(10)	//�`���[�W�V���b�g�G�t�F�N�g�̃��C�t

//*****************************************************************************
//�ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEffect::m_pTexture[EFFECT_TYPE_MAX] = {};

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CEffect::CEffect()
{
	m_EffectType = EFFECT_TYPE_NONE;
	m_Animation = ANIMATION_UNREPEAT;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SetSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nLifeCounter = 0;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CEffect::~CEffect()
{
}

//*****************************************************************************
//���[�h����
//*****************************************************************************
HRESULT CEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect00.png", &m_pTexture[EFFECT_TYPE_GET_SCORE]);
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect01.png", &m_pTexture[EFFECT_TYPE_GET_SPEED_UP]);
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect02.png", &m_pTexture[EFFECT_TYPE_GET_TWIN_BULLET]);
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect03.png", &m_pTexture[EFFECT_TYPE_GET_BARRIER]);
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect04.png", &m_pTexture[EFFECT_TYPE_BARRIER]);
	D3DXCreateTextureFromFile(pDevice, "Textures/effect/effect05.png", &m_pTexture[EFFECT_TYPE_CHARGE_SHOT]);

	return S_OK;
}

//*****************************************************************************
//�A�����[�h����
//*****************************************************************************
void CEffect::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCount = 0; nCount < EFFECT_TYPE_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//�G�t�F�N�g�̐���
//*****************************************************************************
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fEffectSpeed, EFFECT_TYPE EffectType, ANIMATION Animation, CSound::SOUND_LABEL SoundLabel)
{
	//�G�t�F�N�g�̃������m��
	CEffect *pEffect = new CEffect;

	//�G�t�F�N�g�̏���������
	pEffect->Init(pos, size, fEffectSpeed, EffectType, Animation);

	switch (EffectType)
	{
	case EFFECT_TYPE_GET_SCORE:
	case EFFECT_TYPE_GET_SPEED_UP:
	case EFFECT_TYPE_GET_TWIN_BULLET:
	case EFFECT_TYPE_GET_BARRIER:

		//�T�E���h�̍Đ�
		CManager::GetSound()->Play(SoundLabel);

		break;

	default:

		break;
	}

	return pEffect;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fEffectSpeed, EFFECT_TYPE EffectType, ANIMATION Animation)
{
	//�G�t�F�N�g�^�C�v�̐ݒ�
	m_EffectType = EffectType;

	//�A�j���[�V�����̐ݒ�
	m_Animation = Animation;

	//�v���C���[�|�W�V�����̎擾
	D3DXVECTOR3 PlayerPos = CManager::GetPlayer()->GetPos();

	//�ʒu�̐ݒ�
	m_pos = pos;

	//�T�C�Y�̐ݒ�
	SetSize(size);

	switch (m_EffectType)
	{
	case EFFECT_TYPE_GET_SCORE:

		//�ʒu�̐ݒ�
		SetPos(pos + PlayerPos);

		//�G�t�F�N�g�e�N�X�`���̃A�j���[�V����
		InitAnimation(fEffectSpeed, 5.0f, 3.0f);

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_EFFECT);

		break;

	case EFFECT_TYPE_GET_SPEED_UP:

		//�ʒu�̐ݒ�
		SetPos(pos + PlayerPos);

		//�G�t�F�N�g�e�N�X�`���̃A�j���[�V����
		InitAnimation(fEffectSpeed, 7.0f, 7.0f);

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_EFFECT);

		break;

	case EFFECT_TYPE_GET_TWIN_BULLET:

		//�ʒu�̐ݒ�
		SetPos(pos + PlayerPos);

		//�G�t�F�N�g�e�N�X�`���̃A�j���[�V����
		InitAnimation(fEffectSpeed, 7.0f, 7.0f);

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_EFFECT);

		break;

	case EFFECT_TYPE_GET_BARRIER:

		//�ʒu�̐ݒ�
		SetPos(pos + PlayerPos);

		//�G�t�F�N�g�e�N�X�`���̃A�j���[�V����
		InitAnimation(fEffectSpeed, 7.0f, 7.0f);

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_EFFECT);

		break;

	case EFFECT_TYPE_CHARGE_SHOT:

		//�ʒu�̐ݒ�
		SetPos(pos);

		//�����T�C�Y�̐ݒ�
		m_size = size;

		//�ύX�T�C�Y�̐ݒ�
		m_SetSize = size;

		//�����J���[�̐ݒ�
		m_color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//�I�u�W�F�N�g�̎�ނ̐ݒ�
		SetObjType(OBJTYPE_EFFECT_BULLET);

		//�T�C�Y�̐ݒ�
		SetSize(m_size);

		break;

	default:

		break;
	}

	//�e�N�X�`���̐ݒ�
	BindTexture(m_pTexture[EffectType]);

	//�V�[��2D�̏���������
	CScene2D::Init();

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CEffect::Uninit(void)
{
	//�V�[��2D�̏I������
	CScene2D::Uninit();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CEffect::Update(void)
{
	//�v���C���[�|�W�V�����̎擾
	D3DXVECTOR3 PlayerPos = CManager::GetPlayer()->GetPos();

	switch (m_EffectType)
	{
	case EFFECT_TYPE_GET_SCORE:
	case EFFECT_TYPE_GET_SPEED_UP:
	case EFFECT_TYPE_GET_TWIN_BULLET:
	case EFFECT_TYPE_GET_BARRIER:

		//�|�W�V�����̍X�V
		SetPos(m_pos + PlayerPos);

		break;

	case EFFECT_TYPE_CHARGE_SHOT:

		m_nLifeCounter++;

		//���l�ƃT�C�Y��������
		m_color.a -= (1.0f / (float)SHOT_EFFECT_LIFE);
		m_SetSize -= (m_size / (float)SHOT_EFFECT_LIFE);

		//�J���[�̐ݒ�
		SetColor(m_color);

		//�T�C�Y�̐ݒ�
		SetSize(m_SetSize);

		//�����Ȃ��Ȃ����Ƃ�
		if (m_nLifeCounter >= SHOT_EFFECT_LIFE)
		{
			m_nLifeCounter = 0;

			//�I������
			Uninit();
			return;
		}

		break;

	default:

		break;
	}

	//�V�[��2D�̍X�V����
	CScene2D::Update();

	//�A�j���[�V�����̍X�V����
	CScene2D::UpdateAnimation(m_Animation, NOT_INVERSION);
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	switch (m_EffectType)
	{
	case EFFECT_TYPE_CHARGE_SHOT:

		pDevice = CManager::GetRenderer()->GetDevice();

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//�V�[��2D�̕`�揈��
		CScene2D::Draw();

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		break;

	default:

		//�V�[���̕`�揈��
		CScene2D::Draw();

		break;
	}
}

//*****************************************************************************
//�A�j���[�V�����̐ݒ�
//*****************************************************************************
void CEffect::SetAnimation(ANIMATION Animation)
{
	m_Animation = Animation;
}