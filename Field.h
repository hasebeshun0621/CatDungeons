#pragma once

#include	<SimpleMath.h>
#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"dx11mathutil.h"
#include	"CPlaneMesh.h"
#include	"CMeshRenderer.h"

//�@�t�B�[���h��
constexpr float FieldX = 480;
//�@�t�B�[���h��
constexpr float FieldZ = 320;
//�@�������x
constexpr float FallY = 70;

//�@�t�B�[���h��������
void InitField();
//�@�t�B�[���h�`�揈��
void DrawField();

//�G���A��O���� true�O	false��
bool FieldOutCheck(DirectX::XMFLOAT3 pos);

std::vector<VERTEX_3D> GetSquareVertices(int SquareNo);
