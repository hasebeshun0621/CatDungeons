#pragma once
#include	<directxmath.h>
#include	"renderer.h"
#include	"CPlane.h"
#include	"CMeshRenderer.h"
#include	"CBoxMesh.h"
#include	"CBoundingBox.h"

#include	"Enemy.h"

//�@�J�M�\����
struct Key{

	int sts = -1;//-1����  1�h���b�v

	DirectX::SimpleMath::Vector3 position = { 0,0,0 };			//�ʒu
	DirectX::SimpleMath::Vector3 rotation = { 0,0,0 };			//��]
	DirectX::SimpleMath::Vector3 scale = { 10,14,10 };			//�X�P�[��

};

//�@�J�M��������
void InitKey();
//�@�J�M�X�V����
void UpdateKey();
//�@�J�M�`�揈��
void DrawKey();
//�@�J�M�I������
void ExitKey();

//�@�J�M�}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxKey();

//�@�J�M�h���b�v
void KeyDrop(int idx);
//�@�J�M�����h���b�v
void FallKeyDrop(int idx);
//�@�v���C���[�ƃJ�M�̓����蔻��
bool HitCheckKey();

