#pragma once

#include	"camera.h"
#include	"TitleUI.h"
#include	"TitleUI_push.h"
#include	"TitleBG.h"
#include	"TitlePlayer.h"
#include	"TitleEnemy.h"

//　タイトルシーン初期処理
void InitTitleScene();
//　タイトルシーン更新処理
void UpdateTitleScene();
//　タイトルシーン描画処理
void DrawTitleScene();
//　タイトルシーン終了処理
void ExitTitleScene();