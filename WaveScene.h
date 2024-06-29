#pragma once

#include	"XAudio2.h"
#include	"scene.h"
#include	"camera.h"

#include	"Player.h"
#include	"PlayerBullet.h"
#include	"CatchPointer.h"

#include	"Enemy.h"
#include	"EnemyBullet.h"

#include	"Key.h"
#include	"HPPortion.h"

#include	"field.h"
#include	"FieldWall.h"
#include	"wall.h"
#include	"Poison.h"

#include	"UI.h"
#include	"Score.h"
#include	"particle.h"

static int WaveMax = 3;

//　ウェーブシーン初期処理
void InitWave();
//　ウェーブシーン更新処理
void UpdateWave();
//　ウェーブシーン描画処理
void DrawWave();
//　ウェーブシーン終了処理
void ExitWave();

//　ヒットストップFcntを設定
void SetHitStop(int cnt);
//ヒットストップをかける
bool HitStop();

//　鍵を拾うと次のステージへ
void NexWave();
//　ボスウェーブかチェック
bool NexBossCheck();
//　ゲームオーバーをチェック
void GameOverCheck();

//　今のウェーブを取得
int GetNowWave();