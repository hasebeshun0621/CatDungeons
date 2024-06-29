#include "Tutorial.h"

void InitTutorial() {
	InitTutorialWalk();

}

void UpdateTutorial() {
static bool nextscene = false;
static int tutorialcheck = 0;

UpdateTutorialWalk();
	//プレイヤーの歩行操作チェック
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W) ||
		CDirectInput::GetInstance().CheckKeyBuffer(DIK_A) ||
		CDirectInput::GetInstance().CheckKeyBuffer(DIK_S) ||
		CDirectInput::GetInstance().CheckKeyBuffer(DIK_D) &&
		tutorialcheck==0)
	{
		tutorialcheck = 1;
	}

	//プレイヤーのダッシュ操作をチェック
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
