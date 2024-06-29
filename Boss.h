#pragma once
#include	<directxmath.h>
#include	"Collision.h"
#include	"renderer.h"

#include	"Player.h"
#include	"BossBullet.h"
#include	"Key.h"
#include	"HPPortion.h"
#include	"Field.h"

//　ボス最大数
constexpr int Boss_Max = 1;

//　ボスモデル最大数
constexpr int BossMod_Max = 2;

//　ボス体力
constexpr int ENEMYHP_BOSS = 40;

//　ボス構造体
struct Boss
{
	int sts = 0;															//-1,死　１生存中
	int Type = 0;															//ボスTYPE
	DirectX::XMFLOAT3	position = { 0,0,0 };								//位置
	DirectX::XMFLOAT3	rotation = { 0,0,0 };								//回転
	DirectX::XMFLOAT3	scale = { 0,0,0 };									//サイズ
	DirectX::SimpleMath::Vector3	destrot = { 0.0f,0.0f,0.0f };			// 目標回転角度
	float RATE_ROTATE_ENEMY = 0.05f;										// 回転慣性係数
	float MoveSpeed = 0.0f;													//移動速度

	int HP = 0;								//体力

	bool KeyGet = false;					//鍵を持っているか
	bool ItemGet = false;					//アイテムを持っているか

	bool KnockSwitch = false;				//ノックバックオンオフ
	int Knockfcnt = 0;						//ノックバックカウント

	bool DeathSwitch = false;				//死亡演出オンオフ
	DirectX::SimpleMath::Matrix pbDeat;		//ヒットした弾の進行方向を取得

	int fcnt = 0;							//発射用カウンター
	int fullcnt = 0;						//弾発射間隔

};

//　ボス初期化
Boss InitializBoss();

//　ボス初期処理
void InitBoss();
//　ボス更新処理
void UpdateBoss();
//　ボス描画処理
void DrawBoss();
//　ボス終了処理
void ExitBoss();

//　ボスポジション取得
DirectX::XMFLOAT3 GetBossPosition();
//　ボス回転取得
DirectX::XMFLOAT3 GetBossRotation();
//　ボス回転目標を取得
DirectX::XMFLOAT3 GetBossDestrot();
//　ボスマトリックス取得
DirectX::SimpleMath::Matrix GetMtxBoss();
//　ボスOBB取得
BoundingBoxOBB GetBossOBB();

//　ボスステータス取得
int GetStsBoss();

//　ボスをプレイヤーに向ける
void BossLookPlayer();
//　ボス移動
void BossMove();
//　ボスノックバック
void BossKnockBack(int idx);
//　ボス落下
void BossFall();

//　ボスのHPを減らす
void BossHPReduce(int dame);
//　ボス死亡チェック
void BossDeat(int bidx);
//　ボス死亡演出
void BossDeathAnime();



