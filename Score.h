#pragma once

#include	<d3d11.h>
#include	<SimpleMath.h>
#include	<WICTextureLoader.h>

#include	"dx11mathutil.h"
#include	"renderer.h"
#include	"CShader.h"
#include	"camera.h"
#include	"CMaterial.h"

// �X�R�A������
void InitScore();
// �X�R�A�X�V
void UpdateScore();
// �X�R�A�`��
void DrawScore();
void ExitScore();

void ScoreUp(int up);