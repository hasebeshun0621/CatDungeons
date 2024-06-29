#pragma once

#include	<DirectXMath.h>
#include	<d3d11.h>
#include	<wrl/client.h>
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

//　ボディパーティクル初期処理
void InitBodyParticle();
//　ボディパーティクル更新処理
void UpdateBodyParticle();
//　ボディパーティクル描画処理
void DrawBodyParticle();
//　ボディパーティクル終了処理
void ExitBodyParticle();

//　エフェクトの設定
int SetBodyParticle(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 move, DirectX::XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);

//　回復用パーティクル
void MakeHealParticle(DirectX::XMFLOAT3 pos);
//　ダメージパーティクル
void MakeDameParticle(DirectX::XMFLOAT3 pos);
//　弾発射パーティクル
void MakeBulletFireParticle(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 col);
