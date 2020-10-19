//=============================================================================
//
// �o���b�g���� [bullet.cpp]
// Author : Suzuki Mahiro
//
//=============================================================================

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "bullet.h"
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "sound.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include "score.h"
#include "enemy.h"
#include "effect.h"
#include <time.h>
#include "score_attack.h"
#include "tutorial_score_attack.h"

//*****************************************************************************
//�ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CBullet::CBullet()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fGravity = 0.0f;
	m_BulletState = BULLET_STATE_NONE;
}

//*****************************************************************************
//�f�X�g���N�^
//*****************************************************************************
CBullet::~CBullet()
{
}

//*****************************************************************************
//���[�h����
//*****************************************************************************
HRESULT CBullet::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "Textures/bullet.png", &m_pTexture);

	return S_OK;
}

//*****************************************************************************
//�A�����[�h����
//*****************************************************************************
void CBullet::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//*****************************************************************************
//�o���b�g�̐���
//*****************************************************************************
CBullet *CBullet::Create(D3DXVECTOR3 pos, int nCntCharge)
{
	//�o���b�g�̃������m��
	CBullet *pBullet = new CBullet;

	//�o���b�g�̏���������
	pBullet->Init(pos, nCntCharge);

	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	pBullet->SetObjType(OBJTYPE_BULLET);

	return pBullet;
}

//*****************************************************************************
//����������
//*****************************************************************************
HRESULT CBullet::Init(D3DXVECTOR3 pos, int nCntCharge)
{
	//�T�C�Y�̐ݒ�
	SetSize(nCntCharge);

	//�ʒu�̐ݒ�
	SetPos(pos);

	//�V�[��2D�̏���������
	CScene2D::Init();

	//�e�N�X�`���̐ݒ�
	BindTexture(m_pTexture);

	//�o���b�g�̃`���[�W��Ԃ̐ݒ�
	m_BulletState = BULLET_STATE_NONE;

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************
void CBullet::Uninit(void)
{
	//�V�[��2D�̏I������
	CScene2D::Uninit();
}

//*****************************************************************************
//�X�V����
//*****************************************************************************
void CBullet::Update(void)
{
	D3DXVECTOR3 pos = CScene2D::GetPos();

	//�|�W�V�������ړ�
	pos += m_move;

	//�|�W�V�����̐ݒ�
	SetPos(pos);

	switch (m_BulletState)
	{
	case BULLET_STATE_NONE:	//�ʏ�̒e�������Ƃ�

		break;

	case BULLET_STATE_MAX:	//MAX�`���[�W�̒e�������Ƃ�

		//�G�t�F�N�g�̐���
		CEffect::Create(pos, m_size, 0.0f, CEffect::EFFECT_TYPE_CHARGE_SHOT, ANIMATION_UNREPEAT, CSound::SOUND_LABEL_SE_HIT1);

		break;

	default:

		break;
	}

	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		//�I�u�W�F�N�g�̎擾
		CScene *pScene = GetScene(nCntScene);

		CEnemy *pEnemy = dynamic_cast<CEnemy*>(pScene);

		if (pEnemy != NULL)
		{
			//�I�u�W�F�N�g�^�C�v�̎擾
			OBJTYPE objType = pEnemy->GetObjType();

			srand((unsigned int)time(NULL));
			int ItemPosY = rand() % 2 + 1;

			if (objType == OBJTYPE_BOSS_BALL)
			{ //�{�X�{�[���ɓ��������Ƃ�
				D3DXVECTOR3 targetPos = pEnemy->GetPos();

				if (targetPos.x - (BOSS_BALL_SIZE.x / 2) < pos.x + (m_size.x / 2) &&
					targetPos.x + (BOSS_BALL_SIZE.x / 2) > pos.x - (m_size.x / 2) &&
					targetPos.y - (BOSS_BALL_SIZE.y / 2) < pos.y + (m_size.y / 2) &&
					targetPos.y + (BOSS_BALL_SIZE.y / 2) > pos.y - (m_size.y / 2))
				{
					if (m_BulletState != BULLET_STATE_NOT_SHOOT)
					{
						//SE�̍Đ�
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT2);

						switch (m_BulletState)
						{
						case BULLET_STATE_NONE:	//�ʏ�̒e�������Ƃ�

							//�{�[���̃��C�t�����炷
							pEnemy->SetBallLife(1);

							break;

						case BULLET_STATE_MAX:	//MAX�`���[�W�̒e�������Ƃ�

							//�{�[���̃��C�t�����炷
							pEnemy->SetBallLife(3);

							break;

						default:

							break;
						}

						//�{�[���̃��C�t���擾
						int nBallLife = pEnemy->GetBallLife();

						if (nBallLife <= 0)
						{
							//�t�@�[�X�g�{�[���̐���
							CEnemy::Create(D3DXVECTOR3(targetPos.x + BALL_DIVISION_POS.x, targetPos.y + BALL_DIVISION_POS.y, 0.0f), FIRST_BALL_SIZE, D3DXVECTOR3(3.0f, -1.0f, 0.0f), OBJTYPE_FIRST_BALL);
							CEnemy::Create(D3DXVECTOR3(targetPos.x - BALL_DIVISION_POS.x, targetPos.y + BALL_DIVISION_POS.y, 0.0f), FIRST_BALL_SIZE, D3DXVECTOR3(-3.0f, -1.0f, 0.0f), OBJTYPE_FIRST_BALL);

							switch (m_BulletState)
							{
							case BULLET_STATE_NONE:	//�ʏ�̒e�������Ƃ�

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CScoreAttack::AddScore(1);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CTutorialScoreAttack::AddScore(1);
								}

								break;

							case BULLET_STATE_MAX:	//MAX�`���[�W�̒e�������Ƃ�

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CScoreAttack::AddScore(2);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CTutorialScoreAttack::AddScore(2);
								}

								//�A�C�e���̐���
								switch (ItemPosY)
								{
								case 1:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								case 2:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(-1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								default:

									break;
								}

								break;

							default:

								break;
							}

							//�G�l�~�[�̏I������
							pEnemy->Uninit();
						}
						else
						{
							//��Ԃ̐ݒ�
							pEnemy->SetBallState(CEnemy::STATE_DAMAGE);
						}

						//�����̐���
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//�o���b�g�̏I������
						Uninit();
						return;

						break;
					}
					else
					{
						//�����̐���
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//�o���b�g�̏I������
						Uninit();
						return;

						break;
					}
				}
			}
			else if (objType == OBJTYPE_FIRST_BALL)
			{	//�t�@�[�X�g�{�[���ɓ��������Ƃ�
				D3DXVECTOR3 targetPos = pEnemy->GetPos();

				if (targetPos.x - (FIRST_BALL_SIZE.x / 2) < pos.x + (m_size.x / 2) &&
					targetPos.x + (FIRST_BALL_SIZE.x / 2) > pos.x - (m_size.x / 2) &&
					targetPos.y - (FIRST_BALL_SIZE.y / 2) < pos.y + (m_size.y / 2) &&
					targetPos.y + (FIRST_BALL_SIZE.y / 2) > pos.y - (m_size.y / 2))
				{
					if (m_BulletState != BULLET_STATE_NOT_SHOOT)
					{
						//SE�̍Đ�
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT2);

						//�{�[���̃��C�t�����炷
						pEnemy->SetBallLife(1);

						//�{�[���̃��C�t���擾
						int nBallLife = pEnemy->GetBallLife();

						if (nBallLife == 0 || m_BulletState == BULLET_STATE_MAX)
						{
							//�Z�J���h�{�[���̐���
							CEnemy::Create(D3DXVECTOR3(targetPos.x + BALL_DIVISION_POS.x, targetPos.y + BALL_DIVISION_POS.y, 0.0f), SECOND_BALL_SIZE, D3DXVECTOR3(3.0f, -1.0f, 0.0f), OBJTYPE_SECOND_BALL);
							CEnemy::Create(D3DXVECTOR3(targetPos.x - BALL_DIVISION_POS.x, targetPos.y + BALL_DIVISION_POS.y, 0.0f), SECOND_BALL_SIZE, D3DXVECTOR3(-3.0f, -1.0f, 0.0f), OBJTYPE_SECOND_BALL);

							switch (m_BulletState)
							{
							case BULLET_STATE_NONE:	//�ʏ�̒e�������Ƃ�

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CScoreAttack::AddScore(1);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CTutorialScoreAttack::AddScore(1);
								}

								break;

							case BULLET_STATE_MAX:	//MAX�`���[�W�̒e�������Ƃ�

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CScoreAttack::AddScore(2);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CTutorialScoreAttack::AddScore(2);
								}

								//�A�C�e���̐���
								switch (ItemPosY)
								{
								case 1:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								case 2:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(-1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								default:

									break;
								}

								break;

							default:

								break;
							}

							//�G�l�~�[�̏I������
							pEnemy->Uninit();
						}
						else
						{
							//��Ԃ̐ݒ�
							pEnemy->SetBallState(CEnemy::STATE_DAMAGE);
						}

						//�����̐���
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//�o���b�g�̏I������
						Uninit();
						return;

						break;
					}
					else
					{
						//�����̐���
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//�o���b�g�̏I������
						Uninit();
						return;

						break;
					}
				}
			}
			else if (objType == OBJTYPE_SECOND_BALL)
			{	//�Z�J���h�{�[���ɓ��������Ƃ�
				D3DXVECTOR3 targetPos = pEnemy->GetPos();

				if (targetPos.x - (SECOND_BALL_SIZE.x / 2) < pos.x + (m_size.x / 2) &&
					targetPos.x + (SECOND_BALL_SIZE.x / 2) > pos.x - (m_size.x / 2) &&
					targetPos.y - (SECOND_BALL_SIZE.y / 2) < pos.y + (m_size.y / 2) &&
					targetPos.y + (SECOND_BALL_SIZE.y / 2) > pos.y - (m_size.y / 2))
				{
					if (m_BulletState != BULLET_STATE_NOT_SHOOT)
					{
						//SE�̍Đ�
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT2);

						//�{�[���̃��C�t�����炷
						pEnemy->SetBallLife(1);

						//�{�[���̃��C�t���擾
						int nBallLife = pEnemy->GetBallLife();

						if (nBallLife == 0 || m_BulletState == BULLET_STATE_MAX)
						{
							//�T�[�h�{�[���̐���
							CEnemy::Create(D3DXVECTOR3(targetPos.x + BALL_DIVISION_POS.x, targetPos.y + BALL_DIVISION_POS.y, 0.0f), THIRD_BALL_SIZE, D3DXVECTOR3(3.0f, -1.0f, 0.0f), OBJTYPE_THIRD_BALL);
							CEnemy::Create(D3DXVECTOR3(targetPos.x - BALL_DIVISION_POS.x, targetPos.y + BALL_DIVISION_POS.y, 0.0f), THIRD_BALL_SIZE, D3DXVECTOR3(-3.0f, -1.0f, 0.0f), OBJTYPE_THIRD_BALL);

							switch (m_BulletState)
							{
							case BULLET_STATE_NONE:	//�ʏ�̒e�������Ƃ�

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CScoreAttack::AddScore(10);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CTutorialScoreAttack::AddScore(10);
								}

								break;

							case BULLET_STATE_MAX:	//MAX�`���[�W�̒e�������Ƃ�

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CScoreAttack::AddScore(20);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CTutorialScoreAttack::AddScore(20);
								}

								//�A�C�e���̐���
								switch (ItemPosY)
								{
								case 1:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								case 2:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(-1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								default:

									break;
								}

								break;

							default:

								break;
							}

							//�G�l�~�[�̏I������
							pEnemy->Uninit();
						}
						else
						{
							//��Ԃ̐ݒ�
							pEnemy->SetBallState(CEnemy::STATE_DAMAGE);
						}

						//�����̐���
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//�o���b�g�̏I������
						Uninit();
						return;

						break;
					}
					else
					{
						//�����̐���
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//�o���b�g�̏I������
						Uninit();
						return;

						break;
					}
				}
			}
			else if (objType == OBJTYPE_THIRD_BALL)
			{ //�T�[�h�{�[���ɓ��������Ƃ�
				D3DXVECTOR3 targetPos = pEnemy->GetPos();

				if (targetPos.x - (THIRD_BALL_SIZE.x / 2) < pos.x + (m_size.x / 2) &&
					targetPos.x + (THIRD_BALL_SIZE.x / 2) > pos.x - (m_size.x / 2) &&
					targetPos.y - (THIRD_BALL_SIZE.y / 2) < pos.y + (m_size.y / 2) &&
					targetPos.y + (THIRD_BALL_SIZE.y / 2) > pos.y - (m_size.y / 2))
				{
					if (m_BulletState != BULLET_STATE_NOT_SHOOT)
					{
						//SE�̍Đ�
						CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT2);

						//�{�[���̃��C�t�����炷
						pEnemy->SetBallLife(1);

						//�{�[���̃��C�t���擾
						int nBallLife = pEnemy->GetBallLife();

						if (nBallLife == 0 || m_BulletState == BULLET_STATE_MAX)
						{
							switch (m_BulletState)
							{
							case BULLET_STATE_NONE:	//�ʏ�̒e�������Ƃ�

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CScoreAttack::AddScore(100);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CTutorialScoreAttack::AddScore(100);
								}

								break;

							case BULLET_STATE_MAX:	//MAX�`���[�W�̒e�������Ƃ�

								if (CManager::GetMode() == CManager::MODE_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CScoreAttack::AddScore(200);
								}
								else if (CManager::GetMode() == CManager::MODE_TUTORIAL_SCORE_ATTACK)
								{
									//�X�R�A�����Z����
									CTutorialScoreAttack::AddScore(200);
								}

								//�A�C�e���̐���
								switch (ItemPosY)
								{
								case 1:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								case 2:

									CItem::Create(D3DXVECTOR3(targetPos.x + ITEM_DIVISION_POS_X, targetPos.y + ITEM_DIVISION_POS_Y, 0.0f), ITEM_SIZE, D3DXVECTOR3(-1.0f, -6.0f, 0.0f), OBJTYPE_ITEM);

									break;

								default:

									break;
								}

								break;

							default:

								break;
							}

							//�G�l�~�[�̏I������
							pEnemy->Uninit();
						}
						else
						{
							//��Ԃ̐ݒ�
							pEnemy->SetBallState(CEnemy::STATE_DAMAGE);
						}

						//�����̐���
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//�o���b�g�̏I������
						Uninit();
						return;

						break;
					}
					else
					{
						//�����̐���
						CExplosion::Create(pos, CSound::SOUND_LABEL_SE_HIT3);

						//�o���b�g�̏I������
						Uninit();
						return;

						break;
					}
				}
			}
		}
	}

	if (pos.y <= -(m_size.y / 2))
	{ //�o���b�g����ʊO�ɏo���Ƃ�
		//�o���b�g�̏I������
		Uninit();
		return;
	}

	//�V�[��2D�̍X�V����
	CScene2D::Update();
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************
void CBullet::Draw(void)
{
	//�V�[���̕`�揈��
	CScene2D::Draw();
}

//*****************************************************************************
//�e�̈ړ��ݒ�
//*****************************************************************************
void CBullet::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//*****************************************************************************
//�e�̃T�C�Y�X�V
//*****************************************************************************
void CBullet::SetSize(int nCntCharge)
{
	int nChargeSpeed = BULLET_CHARGE_SPEED;

	if (CManager::GetPlayer() != NULL)
	{
		nChargeSpeed = CManager::GetPlayer()->GetChargeSpeed();
	}

	m_size.x = (BULLET_SIZE_X / 3) + BULLET_SIZE_X * ((float)nCntCharge / (float)nChargeSpeed);
	m_size.y = (BULLET_SIZE_Y / 3) + BULLET_SIZE_Y * ((float)nCntCharge / (float)nChargeSpeed);

	if (m_size.x <= (BULLET_SIZE_X / 3))
	{
		m_size.x = (BULLET_SIZE_X / 3);
		m_size.y = (BULLET_SIZE_Y / 3);
	}

	//�T�C�Y�̐ݒ�
	CScene2D::SetSize(m_size);
}

//*****************************************************************************
//�o���b�g�^�C�v�̐ݒ�
//*****************************************************************************
void CBullet::SetBulletType(BULLET_STATE BulletState)
{
	m_BulletState = BulletState;
}

//*****************************************************************************
//�o���b�g�T�C�Y�̎擾
//*****************************************************************************
D3DXVECTOR3 CBullet::GetBulletSize(void)
{
	return m_size;
}

//*****************************************************************************
//�o���b�g�̏�Ԃ̎擾
//*****************************************************************************
CBullet::BULLET_STATE CBullet::GetBulletState(void)
{
	return m_BulletState;
}