#pragma once

#include	<directxmath.h>
#include	"camera.h"
#include	"CStaticMeshRenderer.h"

//	タイトルUI構造体
struct TitleUI {

	//メニューの初期位置
	float top = 150.0f;
	float under = 400.0f;

	//タイトルメニューアニメーションでの頂点移動量
	float X_L = 0;
	float X_R = 0;
	float Y_U = 0;
	float Y_D = 0;

};

//　タイトルUI初期処理
void InitTitleUI();
//　タイトルUI更新処理
void UpdateTitleUI();
//　タイトルUI描画処理
void DrawTitleUI();
//　タイトルUI終了処理
void ExitTitleUI();
