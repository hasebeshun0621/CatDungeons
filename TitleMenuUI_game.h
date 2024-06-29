#pragma once

#include	<directxmath.h>
#include	"TitleMenu.h"

//�@�^�C�g�����j���[_�Q�[���\����
struct TitleMenuUI_game {

	//���j���[�̏����ʒu
	float top = 400.0f;
	float under = 500.0f;

	float move = 0.5f;

	//�^�C�g�����j���[�A�j���[�V�����ł̒��_�ړ���
	float X_L = 0;
	float X_R = 0;
	float Y_U = 0;
	float Y_D = 0;

};

//�@�^�C�g�����j���[_�Q�[����������
void InitTitleMenuUI_game();
//�@�^�C�g�����j���[_�Q�[���X�V����
void UpdateTitleMenuUI_game();
//�@�^�C�g�����j���[_�Q�[���`�揈��
void DrawTitleMenuUI_game();
//�@�^�C�g�����j���[_�Q�[���I������
void ExitTitleMenuUI_game();


