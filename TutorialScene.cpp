#include "TutorialScene.h"

void InitTutorialScene() {
	InitUI();
	InitTutorial();
	// カメラ初期化
	Camera::Init();
	

	// プレイヤ初期化
	InitPlayer();
	InitPlayerBullet();
	InitPointer();

	// フィールド初期化
	InitField();
	InitFieldWall();
	InitWall();
	InitPoison();
}

void UpdateTutorialScene() {
	UpdateUI();
	UpdateTutorial();
	// カメラ更新
	Camera::Update();

	UpdatePlayer();
	UpdatePlayerBullet();
	UpdatePointer();
	
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RETURN)){
	//	SetScene(1);
	}

}

void DrawTutorialScene() {
	DrawUI();
	DrawTutorial();
	// カメラセット
	Camera::Draw();

	// プレイヤ描画
	DrawPlayer();
	DrawPlayerBullet();
	DrawPointer();

	// フィールド描画
	DrawField();
	DrawFieldWall();
	DrawWall();
	DrawPoison();
}

void ExitTutorialScene() {
	ExitUI();
	ExitTutorial();

	Camera::Uninit;
	
	ExitPlayer();
	ExitPlayerBullet();
	ExitPointer();
}
