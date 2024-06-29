#pragma once

#include	<SimpleMath.h>
#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"dx11mathutil.h"
#include	"CPlaneMesh.h"
#include	"CMeshRenderer.h"

#include	"CPlane.h"
#include	"Field.h"

//�ǂ̍ő吔
constexpr int FieldWall_Max = 4;

//�@�t�B�[���h�Ǐ�������
void InitFieldWall();
//�@�t�B�[���h�Ǖ`�揈��
void DrawFieldWall();

