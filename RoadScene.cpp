#include	"RoadScene.h"

//�@���[�h�V�[����������
void InitRoadScene() {

	InitRoad();

}

//�@���[�h�V�[���X�V����
void UpdateRoadScene() {

	UpdateRoad();

}

//�@���[�h�V�[���`�揈��
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

//�@���[�h�V�[���I������
void ExitRoadScene() {

	ExitRoad();

}

