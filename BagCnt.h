#pragma once

#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"camera.h"
#include	"CatchPointer.h"

//　バックカウントUI初期処理
void InitBagCnt();
//　バックカウントUI更新処理
void UpdateBagCnt();
//　バックカウントUI描画処理
void DrawBagCnt();
//　バックカウントUI終了処理
void ExitBagCnt();

//　今の所持アイテム数
int NowBagCnt();