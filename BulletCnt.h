#pragma once

#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"camera.h"
#include	"Player.h"

//　バレットカウントUI初期処理
void InitBulletCnt();
//　バレットカウントUI更新処理
void UpdateBulletCnt();
//　バレットカウントUI描画処理
void DrawBulletCnt();
//　バレットカウントUI終了処理
void ExitBulletCnt();

//	今の弾の所持数を取得
void NowBulletCnt(int Bulletcnt);