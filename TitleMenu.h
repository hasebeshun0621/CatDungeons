#pragma once
#include	<directxmath.h>
#include	"camera.h"
#include	"TitleMenuUI_game.h"
#include	"TitleMenuUI_tutorial.h"
//#include	"TitleMenuUI_.h"
#include	"scene.h"

//　タイトルメニュー初期処理
void InitTitleMenu();
//　タイトルメニュー更新処理
void UpdateTitleMenu();
//　タイトルメニュー描画処理
void DrawTitleMenu();
//　タイトルメニュー終了処理
void ExitTitleMenu();

//選んでいるタイトルメニュー
int GetSelect();



