#include	"TitleScene.h"

//�@�^�C�g���V�[����������
void InitTitleScene() {

	Camera::Init();

	PlaySound(TitleBGM);

	InitTitleBG();
	InitTitleUI();
	InitTitleUI_push();
	
	InitTitlePlayer();
	InitTitleEnemy();

}

//�@�^�C�g���V�[���X�V����
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

//�@�^�C�g���V�[���`�揈��
void DrawTitleScene() {
	
	DrawTitleBG();
	DrawTitleUI();
	
	DrawTitlePlayer();
	DrawTitleEnemy();

	DrawTitleUI_push();

}

//�@�^�C�g���V�[���I������
void ExitTitleScene() {
		
	ExitTitleBG();
	ExitTitleUI();
	ExitTitleUI_push();

	ExitTitlePlayer();
	ExitTitleEnemy();

}

