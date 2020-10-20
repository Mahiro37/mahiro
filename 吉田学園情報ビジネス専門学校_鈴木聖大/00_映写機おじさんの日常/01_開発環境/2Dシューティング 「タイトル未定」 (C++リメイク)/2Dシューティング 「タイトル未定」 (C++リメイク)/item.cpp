//=============================================================================
//
// �A�C�e������ [item.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "explosion.h"
#include "bullet.h"
#include "player.h"
#include "tutorial.h"
#include "time.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define ITEM_GRAVITY	(0.18f)	//�A�C�e���̏d��
#define ITEM_ELASTICITY	(-8.0f)	//�A�C�e���̒e��
#define ITEM_CHANGE		(2)		//�A�C�e����؂�ւ���Ԋu

//*****************************************************************************
//�ÓI�����o�ϐ��錾
//*****************************************************************************
CItem *CItem::m_pItem[MAX_ITEM] = {};
LPDIRECT3DTEXTURE9 CItem::m_pTexture[MAX_ITEM_TEXTURE] = {};

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CItem::CItem()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	memset(m_apScene, 0, sizeof(m_apScene));
	m_nItemCounter = 0;
	m_ItemType = ITEMTYPE_NONE;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CItem::~CItem()
{
}

//*****************************************************************************
//���[�h����
//*****************************************************************************
HRESULT CItem::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "Textures/item/item00.png", &m_pTexture[ITEMTYPE_PLAYER_SPEED_UP]);
	D3DXCreateTextureFromFile(pDevice, "Textures/item/item01.png", &m_pTexture[ITEMTYPE_CHARGE_SPEED_UP]);
	D3DXCreateTextureFromFile(pDevice, "Textures/item/item02.png", &m_pTexture[ITEMTYPE_BARRIER]);
	D3DXCreateTextureFromFile(pDevice, "Textures/item/item03.png", &m_pTexture[ITEMTYPE_TIME_EXTENSION_1]);
	D3DXCreateTextureFromFile(pDevice, "Textures/item/item04.png", &m_pTexture[ITEMTYPE_TIME_EXTENSION_2]);

	return S_OK;
}

//*****************************************************************************
//�A�����[�h����
//*****************************************************************************
void CItem::Unload(void)
{
	//�e�N�X�`���̔j��
	for (int nCount = 0; nCount < MAX_ITEM_TEXTURE; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//*****************************************************************************
//�G�l�~�[�̐���
//*****************************************************************************
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, OBJTYPE type)
{
	//�G�l�~�[�̃������m��
	CItem *pItem = new CItem;

	//�G�l�~�[�̏���������
	pItem->Init(pos, size, move);

	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	pItem->SetObjType(type);

	return pItem;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move)
{
	//�G�l�~�[�̐ݒ�
	SetItem(pos, move);

	if (CManager::GetMode() == CManager::MODE_GAME || CManager::GetMode() == CManager::MODE_TUTORIAL_GAME)
	{
		//�A�C�e���^�C�v�̐ݒ�
		m_ItemType = ITEMTYPE_PLAYER_SPEED_UP;

		//�e�N�X�`���̊��蓖��
		BindTexture(m_pTexture[ITEMTYPE_PLAYER_SPEED_UP]);
	}
	else if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK || CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
	{
		//�T�C�Y�̐ݒ�
		size = D3DXVECTOR3(ITEM_SIZE.x, ITEM_SIZE.y * 1.2f, ITEM_SIZE.z);

		//�A�C�e���^�C�v�̐ݒ�
		m_ItemType = ITEMTYPE_TIME_EXTENSION_1;

		//�e�N�X�`���̊��蓖��
		BindTexture(m_pTexture[ITEMTYPE_TIME_EXTENSION_1]);
	}

	//�T�C�Y�̐ݒ�
	SetSize(size);

	//�V�[��2D�̏�����
	CScene2D::Init();

	//�A�C�e���J�E���^�[�̏�����
	m_nItemCounter = 0;

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CItem::Uninit(void)
{
	//�V�[��2D�̏I������
	CScene2D::Uninit();

	//�A�C�e���J�E���^�[�̏�����
	m_nItemCounter = 0;
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CItem::Update(void)
{
	//�|�W�V�����̎擾
	D3DXVECTOR3 pos = CScene2D::GetPos();

	//���̕ǂɓ��������Ƃ�
	if ((pos.x - (ITEM_SIZE.x / 2)) <= WALL_SIZE_X)
	{
		//�ǂ̊O�ɏo����߂�
		pos.x = WALL_SIZE_X + (ITEM_SIZE.x / 2);

		//���˂�����
		m_move.x = m_move.x * -1.0f;

		//�o�E���h���̍Đ�
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_THIRD_BALL);
	}

	//�E�̕ǂɓ��������Ƃ�
	if ((pos.x + (ITEM_SIZE.x / 2)) >= SCREEN_WIDTH - WALL_SIZE_X)
	{
		//�ǂ̊O�ɏo����߂�
		pos.x = (SCREEN_WIDTH - WALL_SIZE_X) - (ITEM_SIZE.x / 2);

		//���˂�����
		m_move.x = m_move.x * -1.0f;

		//�o�E���h���̍Đ�
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_THIRD_BALL);
	}

	//�d�͂����Z
	m_move.y += ITEM_GRAVITY;

	pos += m_move;

	//�|�W�V�����̐ݒ�
	SetPos(pos);

	//�V�[��2D�̍X�V����
	CScene2D::Update();

	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		//�I�u�W�F�N�g�̎擾
		CScene *pScene = GetScene(nCntScene);

		//�_�C�i�~�b�N�L���X�g
		CBullet *pBullet = dynamic_cast<CBullet*>(pScene);	//�w�肵���N���X����Ȃ�������NULL�ɂȂ�

		if (pBullet != NULL)
		{
			D3DXVECTOR3 targetPos = pBullet->GetPos();
			D3DXVECTOR3 BulletSize = pBullet->GetBulletSize();

			//�e�ɓ��������Ƃ�
			if (targetPos.x - (BulletSize.x / 2) < pos.x + (ITEM_SIZE.x / 2) &&
				targetPos.x + (BulletSize.x / 2) > pos.x - (ITEM_SIZE.x / 2) &&
				targetPos.y - (BulletSize.y / 2) < pos.y + (ITEM_SIZE.y / 2) &&
				targetPos.y + (BulletSize.y / 2) > pos.y - (ITEM_SIZE.y / 2))
			{
				if (pBullet->GetBulletState() != CBullet::BULLET_STATE_NOT_SHOOT)
				{
					//SE�̍Đ�
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BOSS_BALL);

					m_nItemCounter++;

					m_move.y = ITEM_ELASTICITY;

					//�����̐���
					CExplosion::Create(pBullet->GetPos(), CSound::SOUND_LABEL_SE_HIT3);

					//�o���b�g�̏I������
					pBullet->Uninit();

					if (CManager::GetMode() == CManager::MODE_GAME || CManager::GetMode() == CManager::MODE_TUTORIAL_GAME)
					{
						switch (m_nItemCounter)
						{
						case 0:

							m_ItemType = ITEMTYPE_PLAYER_SPEED_UP;

							//�e�N�X�`���̍X�V
							BindTexture(m_pTexture[ITEMTYPE_PLAYER_SPEED_UP]);

							break;

						case 1:

							m_ItemType = ITEMTYPE_CHARGE_SPEED_UP;

							//�e�N�X�`���̍X�V
							BindTexture(m_pTexture[ITEMTYPE_CHARGE_SPEED_UP]);

							break;

						case 2:

							m_ItemType = ITEMTYPE_BARRIER;

							//�e�N�X�`���̍X�V
							BindTexture(m_pTexture[ITEMTYPE_BARRIER]);

							break;

						case 3:

							m_nItemCounter = 0;

							m_ItemType = ITEMTYPE_PLAYER_SPEED_UP;

							//�e�N�X�`���̍X�V
							BindTexture(m_pTexture[ITEMTYPE_PLAYER_SPEED_UP]);

							break;

						default:

							break;
						}
					}
					else if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK || CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
					{
						if (m_nItemCounter < 3)
						{ //�A�C�e���J�E���g��3��菬�����Ƃ�
							m_ItemType = ITEMTYPE_TIME_EXTENSION_1;
						}
						else
						{ //�A�C�e���J�E���g��3�ȏ�̂Ƃ�
							m_ItemType = ITEMTYPE_TIME_EXTENSION_2;

							//�e�N�X�`���̍X�V
							BindTexture(m_pTexture[ITEMTYPE_TIME_EXTENSION_2]);
						}
					}

					break;
				}
			}
		}
	}

	
	//��ʂ���ɏo���Ƃ�
	if (pos.y <= -(ITEM_SIZE.y / 2))
	{
		//�㏸�ʂ𔼕��ɂ���
		m_move.y = (m_move.y / 2);
	}
	//��ʂ�艺�ɏo���Ƃ�
	if (pos.y >= SCREEN_HEIGHT + (ITEM_SIZE.y / 2))
	{
		//�A�C�e���̏I������
		Uninit();
	}
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CItem::Draw(void)
{
	//�V�[��2D�̕`�揈��
	CScene2D::Draw();
}

//*****************************************************************************
//�ݒ菈��
//*****************************************************************************
void CItem::SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	m_move = move;

	SetPos(pos);
}

//*****************************************************************************
//�A�C�e���^�C�v�̎擾	
//*****************************************************************************
CItem::ITEMTYPE CItem::GetItemType(void)
{
	return m_ItemType;
}