#pragma once

#include	<directxmath.h>
#include	"TitleMenu.h"

struct TitleMenuUI_tutorial {

	//���j���[�̏����ʒu
	float top = 500.0f;
	float under = 600.0f;

	float move = 0.5f;

	//�^�C�g�����j���[�A�j���[�V�����ł̒��_�ړ���
	float X_L = 0;
	float X_R = 0;
	float Y_U = 0;
	float Y_D = 0;

};

//�@�^�C�g�����j���[_�`���[�g���A����������
void InitTitleMenuUI_tutorial();
//�@�^�C�g�����j���[_�`���[�g���A���X�V����
void UpdateTitleMenuUI_tutorial();
//�@�^�C�g�����j���[_�`���[�g���A���`�揈��
void DrawTitleMenuUI_tutorial();
//�@�^�C�g�����j���[_�`���[�g���A���I������
void ExitTitleMenuUI_tutorial();


