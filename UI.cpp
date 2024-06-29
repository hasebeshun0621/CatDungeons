#include	"UI.h"

//�@UI��������
void InitUI() {

	InitFlameUI();

	InitHPFlameUI();
	InitUIHP();
	InitUIST();
	
	InitTutorialUI();

	InitIcon();
	InitScore();

}

//�@UI�X�V����
void UpdateUI() {

	UpdateFlameUI();

	UpdateHPFlameUI();
	UpdateUIHP();
	UpdateUIST();
	
	UpdateTutorialUI();


	UpdateIcon();
	UpdateScore();

}

//�@UI�`�揈��
void DrawUI() {

	// 2D�`��p�̃J�����������s
	Camera::Draw2D();
	DrawFlameUI();
	DrawHPFlameUI();
	DrawUIHP();
	DrawUIST();
	
	DrawTutorialUI();


	DrawIcon();
	DrawScore();

}

//�@UI�I������
void ExitUI() {

	ExitFlameUI();
	ExitHPFlameUI();

	ExitUIHP();
	ExitUIST();

	ExitTutorialUI();

	ExitIcon();
	ExitScore();

}

