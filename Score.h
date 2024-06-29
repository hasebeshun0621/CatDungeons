#pragma once

#include	<d3d11.h>
#include	<SimpleMath.h>
#include	<WICTextureLoader.h>

#include	"dx11mathutil.h"
#include	"renderer.h"
#include	"CShader.h"
#include	"camera.h"
#include	"CMaterial.h"

// スコア初期化
void InitScore();
// スコア更新
void UpdateScore();
// スコア描画
void DrawScore();
void ExitScore();

void ScoreUp(int up);