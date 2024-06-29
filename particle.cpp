#include	"particle.h"

//　パーティクル初期処理
void InitParticle(){

	InitDashParticle();
	InitBodyParticle();
	InitHitParticle();
	InitCatchParticle();

}

//　パーティクル更新処理
void UpdateParticle(){

	UpdateDashParticle();
	UpdateBodyParticle();
	UpdateHitParticle();
	UpdateCatchParticle();

}

//　パーティクル描画処理
void DrawParticle(){

	Renderer::SetDepthEnable(false);

	DrawDashParticle();
	DrawBodyParticle();
	DrawHitParticle();
	DrawCatchParticle();

	Renderer::SetDepthEnable(true);

}

//　パーティクル終了処理
void ExitParticle(){

	ExitDashParticle();
	ExitBodyParticle();
	ExitHitParticle();
	ExitCatchParticle();

}
