#include "Tutorial.h"

void InitTutorial() {
	InitTutorialWalk();

}

void UpdateTutorial() {
static bool nextscene = false;
static int tutorialcheck = 0;

UpdateTutorialWalk();
	//�v���C���[�̕��s����`�F�b�N
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W) ||
		CDirectInput::GetInstance().CheckKeyBuffer(DIK_A) ||
		CDirectInput::GetInstance().CheckKeyBuffer(DIK_S) ||
		CDirectInput::GetInstance().CheckKeyBuffer(DIK_D) &&
		tutorialcheck==0)
	{
		tutorialcheck = 1;
	}

	//�v���C���[�̃_�b�V��������`�F�b�N
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_L) &&
		tutorialcheck == 1)
	{
		tutorialcheck = 2;
		nextscene = true;
	}

	if (nextscene == true) {
		nextscene = false;
		tutorialcheck = 0;
		//SetScene(2);
	}

}

void DrawTutorial() {
	DrawTutorialWalk();

}

void ExitTutorial() {
	ExitTutorialWalk();

}
