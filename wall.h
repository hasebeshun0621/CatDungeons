#pragma once

#include	<SimpleMath.h>
#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"dx11mathutil.h"
#include	"camera.h"
#include	"CPlaneMesh.h"
#include	"CMeshRenderer.h"
#include	"CPlane.h"

//�ǂ̍ő吔
constexpr int Wall_Max = 4;

//�@�ǂ̉�
static float WallX = 1000;
//�@�ǂ̕�
static float WallZ = 1000;

//�@�Ǐ�������
void InitWall();
//�@�Ǖ`�揈��
void DrawWall();

//�@�}�b�v�O���� true�O	false��
bool MapOutCheck(DirectX::XMFLOAT3 pos);

