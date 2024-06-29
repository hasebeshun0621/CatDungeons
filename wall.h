#pragma once

#include	<SimpleMath.h>
#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"dx11mathutil.h"
#include	"camera.h"
#include	"CPlaneMesh.h"
#include	"CMeshRenderer.h"
#include	"CPlane.h"

//壁の最大数
constexpr int Wall_Max = 4;

//　壁の横
static float WallX = 1000;
//　壁の幅
static float WallZ = 1000;

//　壁初期処理
void InitWall();
//　壁描画処理
void DrawWall();

//　マップ外判定 true外	false中
bool MapOutCheck(DirectX::XMFLOAT3 pos);

