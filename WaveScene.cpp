#include"WaveScene.h"

static	int HitStopCnt = 0;
static	int Wave = 1;

//�@�E�F�[�u�V�[����������
void InitWave() {

	//�@�ŏ��̃E�F�[�u�̂ݏ�����
	if (Wave <= 1) {
		PlaySound(WaveBGM);
		InitUI();
		// �J����������
		Camera::Init();

		// �v���C��������
		InitPlayer();
		InitPlayerBullet();
		InitPointer();
	}

	// �G�l�~�[
	InitEnemy(Wave);
	InitEnemyBullet();

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

//�@�E�F�[�u�V�[���X�V����
void UpdateWave() {
	UpdateUI();

	// �J�����X�V
	Camera::Update();

	if (HitStop()) {
		UpdatePlayer();
		UpdatePointer();

		UpdatePlayerBullet();

		UpdateEnemy();
		UpdateEnemyBullet();

		UpdateKey();
		UpdateHPPortion();

		UpdatePoison();
	}
	UpdateParticle();

	NexWave();
	GameOverCheck();
}

//�@�E�F�[�u�V�[���`�揈��
void DrawWave() {
	// �t�B�[���h�`��
	DrawField();
	DrawFieldWall();
	DrawWall();
	DrawPoison();

	// �v���C���`��
	DrawPlayer();
	DrawPlayerBullet();
	DrawPointer();

	DrawEnemy();
	DrawEnemyBullet();

	DrawKey();
	DrawHPPortion();

	DrawUI();
	// �J�����Z�b�g
	Camera::Draw();

	DrawParticle();

}

//�@�E�F�[�u�V�[���I������
void ExitWave() {

	ExitUI();

	Camera::Uninit;
	
	ExitPlayer();
	ExitPlayerBullet();
	ExitPointer();
	
	ExitEnemy();
	ExitEnemyBullet();

	ExitKey();
	ExitHPPortion();

	ExitParticle();

}

//�@�����E���Ǝ��̃X�e�[�W��
void NexWave() {

	if (HitCheckKey()) {
		Wave++;
		PlaySound(GetKeySE);
		SetScene(ROAD);
	}

}

//�@�{�X�E�F�[�u���`�F�b�N
bool NexBossCheck() {
	
	if (Wave >= WaveMax) {
		Wave = 1;
		return true;
	}

	return false;
}

//�@�Q�[���I�[�o�[���`�F�b�N
void GameOverCheck() {

	if (GetPlayerHp() <= 0) {
		Wave = 1;
		StopSound(WaveBGM);
		StopSound(BossBGM);
		SetScene(GAMEOVER);
	}

}

//�@�q�b�g�X�g�b�vFcnt��ݒ�
void SetHitStop(int cnt) {

	 HitStopCnt += cnt;

}

//�q�b�g�X�g�b�v��������
bool HitStop() {

	if (HitStopCnt == 0) {
		return true;
	}
	else {
		HitStopCnt--;
		return false;
	}

}

//�@���̃E�F�[�u���擾
int GetNowWave() {
	return Wave;
}
