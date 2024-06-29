#pragma once

#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"CDirectInput.h"
#include	"camera.h"

//　操作説明画面構造体
struct TutorialUI {

	//メニューの初期位置
	float top = (SCREEN_HEIGHT/6)*5;
	float under = SCREEN_HEIGHT-10;

};

//　操作説明画面初期処理
void InitTutorialUI();
//　操作説明画面更新処理
void UpdateTutorialUI();
//　操作説明画面描画処理
void DrawTutorialUI();
//　操作説明画面終了処理
void ExitTutorialUI();

bool GetTutorialDrawSwitch();
