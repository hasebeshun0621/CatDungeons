#pragma once
#include	<directxmath.h>
#include	"renderer.h"
#include	"CPlane.h"
#include	"CMeshRenderer.h"
#include	"CBoxMesh.h"
#include	"CBoundingBox.h"

#include	"Enemy.h"

//�@HP�|�[�V�����ő吔
constexpr int HPPortion_Max = 2;

//�@HP�|�[�V�����\����
struct HPPortion {
	
	int sts = -1;											//-1����  1�h���b�v

	DirectX::SimpleMath::Vector3 position = { 0,0,0 };		//�ʒu
	DirectX::SimpleMath::Vector3 rotation = { 0,0,0 };		//��]
	DirectX::SimpleMath::Vector3 scale = { 10,14,10 };		//�X�P�[��

	float num = 0;

};

//�@HP�|�[�V������������
void InitHPPortion();
//�@HP�|�[�V�����X�V����
void UpdateHPPortion();
//�@HP�|�[�V�����`�揈��
void DrawHPPortion();
//�@HP�|�[�V�����I������
void ExitHPPortion();

//�@HP�|�[�V�����}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxHPPortion(int idx);
//�@HP�|�[�V����OBB�擾
BoundingBoxOBB GetHPPortionOBB(int idx);

//�@HP�|�[�V�����h���b�v
void HPPortionDrop(int idx);

//�@HP�|�[�V���������h���b�v
void FallHPPotionDrop(int idx);

//�@�v���C���[��HP�|�[�V�����̓����蔻��
void HitCheckHPPortion();

