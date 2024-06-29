#pragma once

#include	<directxmath.h>
#include	"TitleMenu.h"

//　タイトルメニュー_ゲーム構造体
struct TitleMenuUI_game {

	//メニューの初期位置
	float top = 400.0f;
	float under = 500.0f;

	float move = 0.5f;

	//タイトルメニューアニメーションでの頂点移動量
	float X_L = 0;
	float X_R = 0;
	float Y_U = 0;
	float Y_D = 0;

};

//　タイトルメニュー_ゲーム初期処理
void InitTitleMenuUI_game();
//　タイトルメニュー_ゲーム更新処理
void UpdateTitleMenuUI_game();
//　タイトルメニュー_ゲーム描画処理
void DrawTitleMenuUI_game();
//　タイトルメニュー_ゲーム終了処理
void ExitTitleMenuUI_game();


