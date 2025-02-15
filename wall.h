#pragma once

#include	<SimpleMath.h>
#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"dx11mathutil.h"
#include	"camera.h"
#include	"CPlaneMesh.h"
#include	"CMeshRenderer.h"
#include	"CPlane.h"

//ΗΜΕε
constexpr int Wall_Max = 4;

//@ΗΜ‘
static float WallX = 1000;
//@ΗΜ
static float WallZ = 1000;

//@Ηϊ
void InitWall();
//@Η`ζ
void DrawWall();

//@}bvO»θ trueO	false
bool MapOutCheck(DirectX::XMFLOAT3 pos);

