//=============================================================================
//
// サウンド処理 [XAudio2.h]
//
//=============================================================================
#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include <xaudio2.h>

// サウンドファイル
typedef enum
{
	TitleBGM=0,					// タイトルBGM
	WaveBGM,					// バトルシーンBGM
	BossBGM,					// ボスシーンBGM
	CatchSE,					// バレットキャッチSE
	HitSE,						// 弾が当たったSE
	KOSE,						// 敵を倒すSE
	DameSE,						// 弾をくらったSE
	FireSE,						// 弾を撃つSE
	DashSE,						// ダッシュSE
	FallSE,						// 落下SE
	HealSE,						// 回復SE
	ItemSelectSE,				// アイテム選択SE
	BulletSE,					// バレット選択SE
	GetKeySE,					// 鍵ゲットSE
	GetItemSE,					// アイテムゲットSE
	NetSE,						// 虫網を振るSE
	TitleMSE,					// タイトルメニューセレクトSE
	TitleMEnterSE,				// タイトルメニュー決定SE

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(void);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void PauseSound(SOUND_LABEL label);
#endif
