#pragma once

#include	<directxmath.h>
#include	"TitleMenu.h"

struct TitleMenuUI_tutorial {

	//メニューの初期位置
	float top = 500.0f;
	float under = 600.0f;

	float move = 0.5f;

	//タイトルメニューアニメーションでの頂点移動量
	float X_L = 0;
	float X_R = 0;
	float Y_U = 0;
	float Y_D = 0;

};

//　タイトルメニュー_チュートリアル初期処理
void InitTitleMenuUI_tutorial();
//　タイトルメニュー_チュートリアル更新処理
void UpdateTitleMenuUI_tutorial();
//　タイトルメニュー_チュートリアル描画処理
void DrawTitleMenuUI_tutorial();
//　タイトルメニュー_チュートリアル終了処理
void ExitTitleMenuUI_tutorial();


