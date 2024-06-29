#include	"GameOverScene.h"

//　ゲームオーバーシーン初期処理
void InitGameOverScene() {

	InitGOver();

}

//　ゲームオーバーシーン更新処理
void UpdateGameOverScene() {
	UpdateGOver();
	UpdateUI();
}

//　ゲームオーバーシーン描画処理
void DrawGameOverScene() {
	
	if (GetBeforeScene() == GAME) {
		DrawWave();
	}

	if (GetBeforeScene() == BOSS) {
		DrawBossScene();
	}

	DrawGOver();
	// カメラ描画処理
	Camera::Draw();

}

//　ゲームオーバーシーン終了処理
void ExitGameOverScene() {

	ExitGOver();

}


