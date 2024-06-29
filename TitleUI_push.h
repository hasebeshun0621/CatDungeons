#pragma once

#include	<directxmath.h>
#include	"camera.h"
#include	"CStaticMeshRenderer.h"

//　タイトルUI_push 構造体
struct TitleUI_PushStart {

	//メニューの初期位置
	float top = 550.0f;
	float under = 700.0f;

	float move = 0.5f;

	//タイトルメニューアニメーションでの頂点移動量
	float X_L = 0;
	float X_R = 0;
	float Y_U = 0;
	float Y_D = 0;

};

//　タイトルUI_push初期処理
void InitTitleUI_push();
//　タイトルUI_push更新処理
void UpdateTitleUI_push();
//　タイトルUI_push描画処理
void DrawTitleUI_push();
//　タイトルUI_push終了処理
void ExitTitleUI_push();


