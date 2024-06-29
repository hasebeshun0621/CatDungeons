#pragma once

#include	<SimpleMath.h>
#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"dx11mathutil.h"
#include	"CPlaneMesh.h"
#include	"CMeshRenderer.h"

//　フィールド横
constexpr float FieldX = 480;
//　フィールド幅
constexpr float FieldZ = 320;
//　落下高度
constexpr float FallY = 70;

//　フィールド初期処理
void InitField();
//　フィールド描画処理
void DrawField();

//エリア場外判定 true外	false中
bool FieldOutCheck(DirectX::XMFLOAT3 pos);

std::vector<VERTEX_3D> GetSquareVertices(int SquareNo);
