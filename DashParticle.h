#pragma once

#include	<DirectXMath.h>
#include	<d3d11.h>
#include	<wrl/client.h>
#include	<random>

#include	"renderer.h"
#include	"CDirectInput.h"
#include	"dx11helper.h"
#include	"camera.h"
#include	"CMaterial.h"
#include	"CPlaneMesh.h"
#include	"CShader.h"
#include	"CMeshRenderer.h"
#include	"Player.h"

//　ダッシュパーティクル初期処理
bool InitDashParticle(void);
//　ダッシュパーティクル更新処理
void UpdateDashParticle(void);
//　ダッシュパーティクル描画処理
void DrawDashParticle(void);
//　ダッシュパーティクル終了処理
void ExitDashParticle(void);
//　ダッシュパーティクル設定
int SetDashParticle(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 move, DirectX::XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);
//　ダッシュパーティクル
void MakeDashParticle(DirectX::XMFLOAT3 pos);