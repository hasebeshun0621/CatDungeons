#include "TutorialScene.h"

void InitTutorialScene() {
	InitUI();
	InitTutorial();
	// �J����������
	Camera::Init();
	

	// �v���C��������
	InitPlayer();
	InitPlayerBullet();
	InitPointer();

	// �t�B�[���h������
	InitField();
	InitFieldWall();
	InitWall();
	InitPoison();
}

void UpdateTutorialScene() {
	UpdateUI();
	UpdateTutorial();
	// �J�����X�V
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
	// �J�����Z�b�g
	Camera::Draw();

	// �v���C���`��
	DrawPlayer();
	DrawPlayerBullet();
	DrawPointer();

	// �t�B�[���h�`��
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
