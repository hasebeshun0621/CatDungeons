#include	"UI.h"

//　UI初期処理
void InitUI() {

	InitFlameUI();

	InitHPFlameUI();
	InitUIHP();
	InitUIST();
	
	InitTutorialUI();

	InitIcon();
	InitScore();

}

//　UI更新処理
void UpdateUI() {

	UpdateFlameUI();

	UpdateHPFlameUI();
	UpdateUIHP();
	UpdateUIST();
	
	UpdateTutorialUI();


	UpdateIcon();
	UpdateScore();

}

//　UI描画処理
void DrawUI() {

	// 2D描画用のカメラ処理実行
	Camera::Draw2D();
	DrawFlameUI();
	DrawHPFlameUI();
	DrawUIHP();
	DrawUIST();
	
	DrawTutorialUI();


	DrawIcon();
	DrawScore();

}

//　UI終了処理
void ExitUI() {

	ExitFlameUI();
	ExitHPFlameUI();

	ExitUIHP();
	ExitUIST();

	ExitTutorialUI();

	ExitIcon();
	ExitScore();

}

