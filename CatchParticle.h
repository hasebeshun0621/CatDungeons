#pragma once

#include	<d3d11.h>
#include	<wrl/client.h>
#include	<DirectXMath.h>
#include	<random>

#include	"renderer.h"
#include	"CDirectInput.h"
#include	"dx11helper.h"
#include	"CMaterial.h"
#include	"CPlaneMesh.h"
#include	"CShader.h"
#include	"CMeshRenderer.h"

#include	"camera.h"
#include	"Player.h"


//　キャッチパーティクル初期処理
void InitCatchParticle();
//　キャッチパーティクル更新処理
void UpdateCatchParticle();
//　キャッチパーティクル描画処理
void DrawCatchParticle();
//　キャッチパーティクル終了処理
void ExitCatchParticle();

//　キャッチエフェクトの設定
int SetCatchParticle(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 move, DirectX::XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);

//　キャッチエフェクト
void MakeCatchParticle(DirectX::XMFLOAT3 pos);