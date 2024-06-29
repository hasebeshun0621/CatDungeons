#include	"RoadScene.h"

//　ロードシーン初期処理
void InitRoadScene() {

	InitRoad();

}

//　ロードシーン更新処理
void UpdateRoadScene() {

	UpdateRoad();

}

//　ロードシーン描画処理
void DrawRoadScene() {

	if (GetBeforeScene() == TITLEMENU) {
		DrawTitleMenuBG();
		DrawTitleMenuScene();
	}

	if (GetBeforeScene() ==GAME) {
		DrawWave();
	}
	DrawRoad();
	Camera::Draw();

}

//　ロードシーン終了処理
void ExitRoadScene() {

	ExitRoad();

}

