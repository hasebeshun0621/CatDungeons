#pragma once

#include	<directxmath.h>
#include	"camera.h"
#include	"CStaticMeshRenderer.h"

//�@�^�C�g��UI_push �\����
struct TitleUI_PushStart {

	//���j���[�̏����ʒu
	float top = 550.0f;
	float under = 700.0f;

	float move = 0.5f;

	//�^�C�g�����j���[�A�j���[�V�����ł̒��_�ړ���
	float X_L = 0;
	float X_R = 0;
	float Y_U = 0;
	float Y_D = 0;

};

//�@�^�C�g��UI_push��������
void InitTitleUI_push();
//�@�^�C�g��UI_push�X�V����
void UpdateTitleUI_push();
//�@�^�C�g��UI_push�`�揈��
void DrawTitleUI_push();
//�@�^�C�g��UI_push�I������
void ExitTitleUI_push();


