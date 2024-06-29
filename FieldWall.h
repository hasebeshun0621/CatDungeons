#pragma once

#include	<SimpleMath.h>
#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"dx11mathutil.h"
#include	"CPlaneMesh.h"
#include	"CMeshRenderer.h"

#include	"CPlane.h"
#include	"Field.h"

//壁の最大数
constexpr int FieldWall_Max = 4;

//　フィールド壁初期処理
void InitFieldWall();
//　フィールド壁描画処理
void DrawFieldWall();

