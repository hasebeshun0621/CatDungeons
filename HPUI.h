#pragma once

#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"camera.h"
#include	"Player.h"

//　HPバーUI初期処理
void InitUIHP();
//　HPバーUI更新処理
void UpdateUIHP();
//　HPバーUI描画処理
void DrawUIHP();
//　HPバーUI終了処理
void ExitUIHP();

//HPバーを減らす
void HPReduce();

