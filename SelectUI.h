#pragma once

#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"camera.h"
#include	"Player.h"

//　セレクトアイテムUI初期処理
void InitSelectUI();
//　セレクトアイテムUI更新処理
void UpdateSelectUI();
//　セレクトアイテムUI描画処理
void DrawSelectUI();
//　セレクトアイテムUI終了処理
void ExitSelectUI();