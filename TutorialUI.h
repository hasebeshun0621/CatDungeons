#pragma once

#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"CDirectInput.h"
#include	"camera.h"

//�@���������ʍ\����
struct TutorialUI {

	//���j���[�̏����ʒu
	float top = (SCREEN_HEIGHT/6)*5;
	float under = SCREEN_HEIGHT-10;

};

//�@���������ʏ�������
void InitTutorialUI();
//�@���������ʍX�V����
void UpdateTutorialUI();
//�@���������ʕ`�揈��
void DrawTutorialUI();
//�@���������ʏI������
void ExitTutorialUI();

bool GetTutorialDrawSwitch();
