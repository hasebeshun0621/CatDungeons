#pragma once

#include	<directxmath.h>
#include	"camera.h"
#include	"CStaticMeshRenderer.h"

//	�^�C�g��UI�\����
struct TitleUI {

	//���j���[�̏����ʒu
	float top = 150.0f;
	float under = 400.0f;

	//�^�C�g�����j���[�A�j���[�V�����ł̒��_�ړ���
	float X_L = 0;
	float X_R = 0;
	float Y_U = 0;
	float Y_D = 0;

};

//�@�^�C�g��UI��������
void InitTitleUI();
//�@�^�C�g��UI�X�V����
void UpdateTitleUI();
//�@�^�C�g��UI�`�揈��
void DrawTitleUI();
//�@�^�C�g��UI�I������
void ExitTitleUI();
