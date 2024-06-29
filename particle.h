#pragma once
#include "renderer.h"

#include "DashParticle.h"
#include "BodyParticle.h"
#include "HitParticle.h"
#include "CatchParticle.h"

//　パーティクル初期処理
void InitParticle();
//　パーティクル更新処理
void UpdateParticle();
//　パーティクル描画処理
void DrawParticle();
//　パーティクル終了処理
void ExitParticle();
