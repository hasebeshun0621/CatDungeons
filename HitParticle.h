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

//　ヒットパーティクル初期処理
void InitHitParticle();
//　ヒットパーティクル更新処理
void UpdateHitParticle();
//　ヒットパーティクル描画処理
void DrawHitParticle();
//　ヒットパーティクル終了処理
void ExitHitParticle();

//　ヒットパーティクル設定
int SetHitParticle(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 move, DirectX::XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);

//　ヒットパーティクル
void MakeHitParticle(DirectX::XMFLOAT3 pos);
//　エネミーキルパーティクル
void MakeKillParticle(DirectX::XMFLOAT3 pos);
//　バレットキャッチエフェクト
void MakeBulletCatchParticle(DirectX::XMFLOAT3 pos);


