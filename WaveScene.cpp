#include"WaveScene.h"

static	int HitStopCnt = 0;
static	int Wave = 1;

//　ウェーブシーン初期処理
void InitWave() {

	//　最初のウェーブのみ初期化
	if (Wave <= 1) {
		PlaySound(WaveBGM);
		InitUI();
		// カメラ初期化
		Camera::Init();

		// プレイヤ初期化
		InitPlayer();
		InitPlayerBullet();
		InitPointer();
	}

	// エネミー
	InitEnemy(Wave);
	InitEnemyBullet();

	//アイテム
	InitKey();
	InitHPPortion();

	// フィールド初期化
	InitField();
	InitFieldWall();
	InitWall();
	InitPoison();

	InitParticle();
}

//　ウェーブシーン更新処理
void UpdateWave() {
	UpdateUI();

	// カメラ更新
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

//　ウェーブシーン描画処理
void DrawWave() {
	// フィールド描画
	DrawField();
	DrawFieldWall();
	DrawWall();
	DrawPoison();

	// プレイヤ描画
	DrawPlayer();
	DrawPlayerBullet();
	DrawPointer();

	DrawEnemy();
	DrawEnemyBullet();

	DrawKey();
	DrawHPPortion();

	DrawUI();
	// カメラセット
	Camera::Draw();

	DrawParticle();

}

//　ウェーブシーン終了処理
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

//　鍵を拾うと次のステージへ
void NexWave() {

	if (HitCheckKey()) {
		Wave++;
		PlaySound(GetKeySE);
		SetScene(ROAD);
	}

}

//　ボスウェーブかチェック
bool NexBossCheck() {
	
	if (Wave >= WaveMax) {
		Wave = 1;
		return true;
	}

	return false;
}

//　ゲームオーバーをチェック
void GameOverCheck() {

	if (GetPlayerHp() <= 0) {
		Wave = 1;
		StopSound(WaveBGM);
		StopSound(BossBGM);
		SetScene(GAMEOVER);
	}

}

//　ヒットストップFcntを設定
void SetHitStop(int cnt) {

	 HitStopCnt += cnt;

}

//ヒットストップをかける
bool HitStop() {

	if (HitStopCnt == 0) {
		return true;
	}
	else {
		HitStopCnt--;
		return false;
	}

}

//　今のウェーブを取得
int GetNowWave() {
	return Wave;
}
