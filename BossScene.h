#pragma once

#include	"camera.h"
#include	"WaveScene.h"
#include	"Boss.h"

//　ボスシーン初期処理
void InitBossScene();
//　ボスシーン更新処理
void UpdateBossScene();
//　ボスシーン描画処理
void DrawBossScene();
//　ボスシーン終了処理
void ExitBossScene();

//　ゲームクリアチェック
void GameClearCheck();