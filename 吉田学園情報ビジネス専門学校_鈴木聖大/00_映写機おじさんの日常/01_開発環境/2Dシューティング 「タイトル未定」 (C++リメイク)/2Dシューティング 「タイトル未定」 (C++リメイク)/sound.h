//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Suzuki Mahiro
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
//ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include <XAudio2.h>

//*****************************************************************************
//クラス宣言
//*****************************************************************************
class CSound
{
public:
	CSound();
	~CSound();

	//サウンドファイル
	typedef enum
	{
		SOUND_LABEL_BGM00 = 0,		//タイトルBGM
		SOUND_LABEL_BGM01,			//セレクトBGM
		SOUND_LABEL_BGM02,			//結果画面BGM
		SOUND_LABEL_BGM03,			//ランキングBGM
		SOUND_LABEL_BGM04,			//映写機BGM
		SOUND_LABEL_BGM05,			//ゲームBGM
		SOUND_LABEL_BGM06,			//チュートリアルBGM
		SOUND_LABEL_SE_BULLET_MAX,	//マックスチャージショット音
		SOUND_LABEL_SE_BULLET_NONE,	//通常ショット音
		SOUND_LABEL_SE_HIT1,		//ヒット音１
		SOUND_LABEL_SE_HIT2,		//ヒット音２
		SOUND_LABEL_SE_HIT3,		//ヒット音３
		SOUND_LABEL_SE_EXPLOSION,	//爆発音
		SOUND_LABEL_SE_BOSS_BALL,	//ボスボール音
		SOUND_LABEL_SE_FIRST_BALL,	//ファーストボール音
		SOUND_LABEL_SE_SECOND_BALL,	//セカンドボール音
		SOUND_LABEL_SE_THIRD_BALL,	//サードボール音
		SOUND_LABEL_CLOSE,			//閉じる音
		SOUND_LABEL_OPEN,			//開く音
		SOUND_LABEL_COUNT,			//カウント音
		SOUND_LABEL_STAGE_NUM,		//ステージ番号音
		SOUND_LABEL_DOOR_OPEN,		//ドアを開ける音
		SOUND_LABEL_PAPER_OPEN,		//操作説明を開く音
		SOUND_LABEL_PAPER_CLOSE,	//操作説明を閉じる音
		SOUND_LABEL_GAME_OVER,		//ゲームオーバー音
		SOUND_LABEL_CHARGE,			//チャージ音
		SOUND_LABEL_RANK_IN,		//ランクイン音
		SOUND_LABEL_SCORE,			//スコアカウント音
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

private:
	// パラメータ構造体
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

															// 各音素材のパラメータ
	SOUNDPARAM m_aParam[SOUND_LABEL_MAX] =
	{
		{ "Sounds/BGM/title.wav", -1 },			//タイトルBGM
		{ "Sounds/BGM/select.wav", -1 },		//セレクトBGM
		{ "Sounds/BGM/clear.wav", -1 },			//結果画面BGM
		{ "Sounds/BGM/ranking.wav", -1 },		//ランキングBGM
		{ "Sounds/BGM/film.wav", -1 },			//映写機BGM
		{ "Sounds/BGM/game.wav", -1 },			//ゲームBGM
		{ "Sounds/BGM/tutorial.wav", -1 },		//チュートリアルBGM
		{ "Sounds/SE/bullet_max.wav", 0 },		//マックスチャージショット音
		{ "Sounds/SE/bullet_none.wav", 0 },		//通常ショット音
		{ "Sounds/SE/hit1.wav", 0 },			//ヒット音１
		{ "Sounds/SE/hit2.wav", 0 },			//ヒット音２
		{ "Sounds/SE/hit3.wav", 0 },			//ヒット音３
		{ "Sounds/SE/explosion.wav", 0 },		//爆発音
		{ "Sounds/SE/boss_ball.wav", 0 },		//ボスボール音
		{ "Sounds/SE/first_ball.wav", 0 },		//ファーストボール音
		{ "Sounds/SE/second_ball.wav", 0 },		//セカンドボール音
		{ "Sounds/SE/third_ball.wav", 0 },		//サードボール音
		{ "Sounds/SE/close.wav", 0 },			//閉じる音
		{ "Sounds/SE/open.wav", 0 },			//開く音
		{ "Sounds/SE/count.wav", 0 },			//カウント音
		{ "Sounds/SE/stage_num.wav", 0 },		//ステージ番号音
		{ "Sounds/SE/door_open.wav", 0 },		//ドアを開ける音
		{ "Sounds/SE/paper_open.wav", 0 },		//操作説明を開く音
		{ "Sounds/SE/paper_close.wav", 0 },		//操作説明を閉じる音
		{ "Sounds/SE/game_over.wav", 0 },		//ゲームオーバー音
		{ "Sounds/SE/charge.wav", -1 },			//チャージ音
		{ "Sounds/SE/rank_in.wav", 0 },			//ランクイン音
		{ "Sounds/SE/score.wav", -1 },			//スコアカウント音
	};
};

#endif