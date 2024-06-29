#pragma once
#include	<functional>

#include	"TitleScene.h"
#include	"TitleMenuScene.h"
#include	"TutorialScene.h"
#include	"WaveScene.h"
#include	"BossScene.h"
#include	"RoadScene.h"
#include	"GameOverScene.h"
#include	"GameClear.h"

// シーン名列挙クラス
enum GAMESCENE 
{
	TITLE,			//　タイトル
	TITLEMENU,		//　タイトルメニュー
	TUTORIAL,		//　チュートリアル
	GAME,			//　ゲームwave
	BOSS,			//　ボス戦
	GAMEOVER,		//　ゲームオーバー
	GAMECLEAR,		//　ゲームクリア
	ROAD,			//　ロード画面
	SCENEMAX,		//　シーンMax
};

// シーンテーブル
struct SceneFunctioTable {
	std::function<void(void)> Init;		// 初期処理
	std::function<void(void)> Update;	// 更新処理
	std::function<void(void)> Draw;		// 描画処理
	std::function<void(void)> Exit;		// 終了処理
};

// シーン初期処理
void InitScene();
// シーン更新処理
void UpdateScene();
// シーン描画処理
void DrawScene();
// シーン終了処理
void ExitScene();
// シーンセット
void SetScene(int idx);

//ひとつ前のシーンを取得	
int GetBeforeScene();
//今のシーンを取得
int GetScene();
