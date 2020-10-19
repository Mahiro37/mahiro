//================================================================
//
// �G���� [enemy.h]
// Author : Suzuki Mahiro
//
//================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//****************************************************************
//�C���N���[�h�t�@�C��
//****************************************************************
#include "scene2d.h"

//****************************************************************
//�}�N����`
//****************************************************************
#define BOSS_BALL_SIZE			(D3DXVECTOR3(200.0f,200.0f,0.0f))	//�{�X�{�[���̃T�C�Y
#define FIRST_BALL_SIZE			(D3DXVECTOR3(150.0f,150.0f,0.0f))	//�t�@�[�X�g�{�[���̃T�C�Y
#define SECOND_BALL_SIZE		(D3DXVECTOR3(100.0f,100.0f,0.0f))	//�Z�J���h�{�[���̃T�C�Y
#define THIRD_BALL_SIZE			(D3DXVECTOR3(50.0f,50.0f,0.0f))		//�T�[�h�{�[���̃T�C�Y
#define BOSS_BALL_RADIUS		(100.0f)							//�{�X�{�[���̔��a
#define FIRST_BALL_RADIUS		(75.0f)								//�t�@�[�X�g�{�[���̔��a
#define SECOND_BALL_RADIUS		(50.0f)								//�Z�J���h�{�[���̔��a
#define THIRD_BALL_RADIUS		(25.0f)								//�T�[�h�{�[���̔��a
#define BALL_DIVISION_POS		(D3DXVECTOR3(50.0f,50.0f,0.0f))		//�����̏������W

//****************************************************************
//�N���X��`
//****************************************************************
class CEnemy :public CScene2D
{
public:
	//�{�[���̎�ނ̗񋓌^
	typedef enum
	{
		TYPE_FIRST = 0,	//�t�@�[�X�g�{�[��
		TYPE_SECOND,	//�Z�J���h�{�[��
		TYPE_THIRD,		//�T�[�h�{�[��
		TYPE_BOSS,		//�{�X�{�[��
		TYPE_MAX
	}TYPE;

	//�{�[���̏�Ԃ̗񋓌^
	typedef enum
	{
		STATE_NONE = 0,	//�ʏ�
		STATE_DAMAGE	//�_���[�W
	}ENEMY_STATE;

	CEnemy();
	~CEnemy();
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, OBJTYPE type);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, OBJTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move);
	void SetBallState(ENEMY_STATE state);
	void SetBallLife(int nDamage);
	void Reflection(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void RotationEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move);

	int GetBallLife(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];
	ENEMY_STATE m_EnemyState;
	D3DXVECTOR3 m_BarrierPosOld;
	D3DXVECTOR3 m_move;
	int m_nBarrierLife;
	int m_nBallLife;
	int m_nDamageColorChange;
	int m_nInvincibleTime;
	int m_nCntCreateBall;
};

#endif