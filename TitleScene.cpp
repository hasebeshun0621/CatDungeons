#include	"TitleScene.h"

//　タイトルシーン初期処理
void InitTitleScene() {

	Camera::Init();

	PlaySound(TitleBGM);

	InitTitleBG();
	InitTitleUI();
	InitTitleUI_push();
	
	InitTitlePlayer();
	InitTitleEnemy();

}

//　タイトルシーン更新処理
void UpdateTitleScene() {

	UpdateTitleBG();
	UpdateTitleUI();

	UpdateTitlePlayer();
	UpdateTitleEnemy();

	UpdateTitleUI_push();
	
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN)) {
		PlaySound(TitleMEnterSE);
		SetScene(TITLEMENU);
	}

}

//　タイトルシーン描画処理
void DrawTitleScene() {
	
	DrawTitleBG();
	DrawTitleUI();
	
	DrawTitlePlayer();
	DrawTitleEnemy();

	DrawTitleUI_push();

}

//　タイトルシーン終了処理
void ExitTitleScene() {
		
	ExitTitleBG();
	ExitTitleUI();
	ExitTitleUI_push();

	ExitTitlePlayer();
	ExitTitleEnemy();

}

