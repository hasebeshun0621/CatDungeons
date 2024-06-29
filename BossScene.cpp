#include"BossScene.h"

//　ボスシーン初期処理
void InitBossScene() {

	InitUI();

	// カメラ初期化
	Camera::Init();

	InitEnemy(4);
	InitEnemyBullet();

	InitBoss();
	InitBossBullet();

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

//　ボスシーン更新処理
void UpdateBossScene() {

	UpdateUI();

	// カメラ更新
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

//　ボスシーン描画処理
void DrawBossScene() {

	// フィールド描画
	DrawField();
	DrawFieldWall();
	DrawWall();
	DrawPoison();

	// プレイヤ描画
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

	// カメラセット
	Camera::Draw();

	DrawParticle();

}

//　ボスシーン終了処理
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

//　ゲームクリアチェック
void GameClearCheck() {

	if (GetStsBoss()<=0) {
		StopSound(BossBGM);
		SetScene(GAMECLEAR);
	}

}



