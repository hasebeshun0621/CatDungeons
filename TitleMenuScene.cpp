#include	"TitleMenuScene.h"

void InitTitleMenuScene() {
	InitTitleMenuBG();
	InitTitleMenu();

}

void UpdateTitleMenuScene() {

	UpdateTitleMenuBG();
	UpdateTitleMenu();
	UpdateTitlePlayer();
	UpdateTitleEnemy();
}

void DrawTitleMenuScene() {
	Camera::Draw();
	DrawTitleMenuBG();

	DrawTitlePlayer();
	DrawTitleEnemy();
	DrawTitleMenu();


}

void ExitTitleMenuScene() {
	ExitTitleMenuBG();
	ExitTitleMenu();
	Camera::Uninit();
	ExitTitlePlayer();
	ExitTitleEnemy();
}

