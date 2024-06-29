#pragma once

#include	<directxmath.h>
#include	"CIndexBuffer.h"
#include	"CVertexBuffer.h"
#include	"CMeshRenderer.h"
#include	"scene.h"

//�@�^�C�g���p�G�l�~�[�\����
struct TitleEnemy
{
	//�@�`�悷��G�l�~�[�^�C�v
	int enetype = 0;

	// RTS���
	DirectX::XMFLOAT3 position = { -270.0f,0.0f,0.0f };		// �ʒu
	DirectX::XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };		// �p��
	DirectX::XMFLOAT3 scale = { 15.0f,15.0f,15.0f };		// �g�k
	
};

//�@�^�C�g���p�G�l�~�[��������
void InitTitleEnemy();
//�@�^�C�g���p�G�l�~�[�X�V����
void UpdateTitleEnemy();
//�@�^�C�g���p�G�l�~�[�`�揈��
void DrawTitleEnemy();
//�@�^�C�g���p�G�l�~�[�I������
void ExitTitleEnemy();

