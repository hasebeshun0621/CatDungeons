#include	"particle.h"

//�@�p�[�e�B�N����������
void InitParticle(){

	InitDashParticle();
	InitBodyParticle();
	InitHitParticle();
	InitCatchParticle();

}

//�@�p�[�e�B�N���X�V����
void UpdateParticle(){

	UpdateDashParticle();
	UpdateBodyParticle();
	UpdateHitParticle();
	UpdateCatchParticle();

}

//�@�p�[�e�B�N���`�揈��
void DrawParticle(){

	Renderer::SetDepthEnable(false);

	DrawDashParticle();
	DrawBodyParticle();
	DrawHitParticle();
	DrawCatchParticle();

	Renderer::SetDepthEnable(true);

}

//�@�p�[�e�B�N���I������
void ExitParticle(){

	ExitDashParticle();
	ExitBodyParticle();
	ExitHitParticle();
	ExitCatchParticle();

}
