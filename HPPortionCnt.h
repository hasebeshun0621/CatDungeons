#pragma once

#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"camera.h"
#include	"Player.h"

//　HPポーションカウントUI初期処理
void InitHPPortionCnt();
//　HPポーションカウントUI更新処理
void UpdateHPPortionCnt();
//　HPポーションカウントUI描画処理
void DrawHPPortionCnt();
//　HPポーションカウントUI終了処理
void ExitHPPortionCnt();

//	今のHPポーションの所持数を取得
void NowHPPortionCnt(int HPPortionCount);
