//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
//�w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include <XAudio2.h>

//*****************************************************************************
//�N���X�錾
//*****************************************************************************
class CSound
{
public:
	CSound();
	~CSound();

	//�T�E���h�t�@�C��
	typedef enum
	{
		SOUND_LABEL_BGM00 = 0,		//�^�C�g��BGM
		SOUND_LABEL_BGM01,			//�Z���N�gBGM
		SOUND_LABEL_BGM02,			//���ʉ��BGM
		SOUND_LABEL_BGM03,			//�����L���OBGM
		SOUND_LABEL_BGM04,			//�f�ʋ@BGM
		SOUND_LABEL_BGM05,			//�Q�[��BGM
		SOUND_LABEL_BGM06,			//�`���[�g���A��BGM
		SOUND_LABEL_SE_BULLET_MAX,	//�}�b�N�X�`���[�W�V���b�g��
		SOUND_LABEL_SE_BULLET_NONE,	//�ʏ�V���b�g��
		SOUND_LABEL_SE_HIT1,		//�q�b�g���P
		SOUND_LABEL_SE_HIT2,		//�q�b�g���Q
		SOUND_LABEL_SE_HIT3,		//�q�b�g���R
		SOUND_LABEL_SE_EXPLOSION,	//������
		SOUND_LABEL_SE_BOSS_BALL,	//�{�X�{�[����
		SOUND_LABEL_SE_FIRST_BALL,	//�t�@�[�X�g�{�[����
		SOUND_LABEL_SE_SECOND_BALL,	//�Z�J���h�{�[����
		SOUND_LABEL_SE_THIRD_BALL,	//�T�[�h�{�[����
		SOUND_LABEL_CLOSE,			//���鉹
		SOUND_LABEL_OPEN,			//�J����
		SOUND_LABEL_COUNT,			//�J�E���g��
		SOUND_LABEL_STAGE_NUM,		//�X�e�[�W�ԍ���
		SOUND_LABEL_DOOR_OPEN,		//�h�A���J���鉹
		SOUND_LABEL_PAPER_OPEN,		//����������J����
		SOUND_LABEL_PAPER_CLOSE,	//�����������鉹
		SOUND_LABEL_GAME_OVER,		//�Q�[���I�[�o�[��
		SOUND_LABEL_CHARGE,			//�`���[�W��
		SOUND_LABEL_RANK_IN,		//�����N�C����
		SOUND_LABEL_SCORE,			//�X�R�A�J�E���g��
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

private:
	// �p�����[�^�\����
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y

															// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aParam[SOUND_LABEL_MAX] =
	{
		{ "Sounds/BGM/title.wav", -1 },			//�^�C�g��BGM
		{ "Sounds/BGM/select.wav", -1 },		//�Z���N�gBGM
		{ "Sounds/BGM/clear.wav", -1 },			//���ʉ��BGM
		{ "Sounds/BGM/ranking.wav", -1 },		//�����L���OBGM
		{ "Sounds/BGM/film.wav", -1 },			//�f�ʋ@BGM
		{ "Sounds/BGM/game.wav", -1 },			//�Q�[��BGM
		{ "Sounds/BGM/tutorial.wav", -1 },		//�`���[�g���A��BGM
		{ "Sounds/SE/bullet_max.wav", 0 },		//�}�b�N�X�`���[�W�V���b�g��
		{ "Sounds/SE/bullet_none.wav", 0 },		//�ʏ�V���b�g��
		{ "Sounds/SE/hit1.wav", 0 },			//�q�b�g���P
		{ "Sounds/SE/hit2.wav", 0 },			//�q�b�g���Q
		{ "Sounds/SE/hit3.wav", 0 },			//�q�b�g���R
		{ "Sounds/SE/explosion.wav", 0 },		//������
		{ "Sounds/SE/boss_ball.wav", 0 },		//�{�X�{�[����
		{ "Sounds/SE/first_ball.wav", 0 },		//�t�@�[�X�g�{�[����
		{ "Sounds/SE/second_ball.wav", 0 },		//�Z�J���h�{�[����
		{ "Sounds/SE/third_ball.wav", 0 },		//�T�[�h�{�[����
		{ "Sounds/SE/close.wav", 0 },			//���鉹
		{ "Sounds/SE/open.wav", 0 },			//�J����
		{ "Sounds/SE/count.wav", 0 },			//�J�E���g��
		{ "Sounds/SE/stage_num.wav", 0 },		//�X�e�[�W�ԍ���
		{ "Sounds/SE/door_open.wav", 0 },		//�h�A���J���鉹
		{ "Sounds/SE/paper_open.wav", 0 },		//����������J����
		{ "Sounds/SE/paper_close.wav", 0 },		//�����������鉹
		{ "Sounds/SE/game_over.wav", 0 },		//�Q�[���I�[�o�[��
		{ "Sounds/SE/charge.wav", -1 },			//�`���[�W��
		{ "Sounds/SE/rank_in.wav", 0 },			//�����N�C����
		{ "Sounds/SE/score.wav", -1 },			//�X�R�A�J�E���g��
	};
};

#endif