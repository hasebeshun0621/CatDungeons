#include	"GameOverScene.h"

//�@�Q�[���I�[�o�[�V�[����������
void InitGameOverScene() {

	InitGOver();

}

//�@�Q�[���I�[�o�[�V�[���X�V����
void UpdateGameOverScene() {
	UpdateGOver();
	UpdateUI();
}

//�@�Q�[���I�[�o�[�V�[���`�揈��
void DrawGameOverScene() {
	
	if (GetBeforeScene() == GAME) {
		DrawWave();
	}

	if (GetBeforeScene() == BOSS) {
		DrawBossScene();
	}

	DrawGOver();
	// �J�����`�揈��
	Camera::Draw();

}

//�@�Q�[���I�[�o�[�V�[���I������
void ExitGameOverScene() {

	ExitGOver();

}


