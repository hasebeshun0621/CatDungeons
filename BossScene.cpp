#include"BossScene.h"

//�@�{�X�V�[����������
void InitBossScene() {

	InitUI();

	// �J����������
	Camera::Init();

	InitEnemy(4);
	InitEnemyBullet();

	InitBoss();
	InitBossBullet();

	//�A�C�e��
	InitKey();
	InitHPPortion();

	// �t�B�[���h������
	InitField();
	InitFieldWall();
	InitWall();
	InitPoison();

	InitParticle();
}

//�@�{�X�V�[���X�V����
void UpdateBossScene() {

	UpdateUI();

	// �J�����X�V
	Camera::Update();

	if (HitStop()) {
		UpdatePlayer();
		UpdatePlayerBullet();
		UpdatePointer();

		UpdateBoss();
		UpdateBossBullet();

		UpdateEnemy();
		UpdateEnemyBullet();

		UpdateKey();
		UpdateHPPortion();

		UpdatePoison();
	}

	UpdateParticle();

	GameOverCheck();
	GameClearCheck();

}

//�@�{�X�V�[���`�揈��
void DrawBossScene() {

	// �t�B�[���h�`��
	DrawField();
	DrawFieldWall();
	DrawWall();
	DrawPoison();

	// �v���C���`��
	DrawPlayer();
	DrawPlayerBullet();
	DrawPointer();

	DrawBoss();
	DrawBossBullet();
	
	DrawEnemy();
	DrawEnemyBullet();

	DrawKey();
	DrawHPPortion();

	DrawUI();

	// �J�����Z�b�g
	Camera::Draw();

	DrawParticle();

}

//�@�{�X�V�[���I������
void ExitBossScene() {

	ExitUI();

	Camera::Uninit;

	ExitPlayer();
	ExitPlayerBullet();
	ExitPointer();

	ExitBoss();
	ExitBossBullet();

	ExitEnemy();
	ExitEnemyBullet();

	ExitKey();
	ExitHPPortion();

	ExitParticle();

}

//�@�Q�[���N���A�`�F�b�N
void GameClearCheck() {

	if (GetStsBoss()<=0) {
		StopSound(BossBGM);
		SetScene(GAMECLEAR);
	}

}



