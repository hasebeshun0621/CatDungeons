#pragma once
#include	<directxmath.h>
#include	"Collision.h"

#include	"Player.h"
#include	"EnemyBullet.h"
#include	"Key.h"
#include	"HPPortion.h"
#include	"Field.h"

//　エネミー最大数
constexpr int Enemy_Max = 100;

//　SuraのHP設定
constexpr int ENEMYHP_Sura = 20;
//　SuragreenのHP設定
constexpr int ENEMYHP_SuraGreen = 30;

enum EnemyMod
{
	Sura = 0,		//青スライム
	SraGreen,		//グリーンスライム
	EnemyMod_Max
};

//　エネミー構造体
struct Enemy
{
	int sts = 0;															//　-1死　1生存中

	int EnemyType = 0;														//　エネミータイプ識別

	DirectX::XMFLOAT3	position = { 0,0,0 };								//位置
	DirectX::XMFLOAT3	rotation = { 0,0,0 };								//回転
	DirectX::XMFLOAT3	scale	 = { 0,0,0 };								//サイズ
	DirectX::SimpleMath::Vector3	destrot = { 0.0f,0.0f,0.0f };			// 目標回転角度
	float RATE_ROTATE_ENEMY = 0.05f;										// 回転慣性係数

	int HP = 0;								//エネミー体力
	bool KeyGet = false;					//鍵を持っているか
	bool ItemGet = false;					//アイテムを持っているか

	bool KnockSwitch = false;				//ノックバックオンオフ
	int Knockfcnt = 0;						//ノックバックカウント
	
	bool DeathSwitch = false;				//死亡演出オンオフ
	DirectX::SimpleMath::Matrix pbDeat;		//ヒットした弾の進行方向を取得

	int fcnt = 0;							//発射用カウンター
	int fullcnt = 0;						//弾発射間隔

};

//　エネミー初期処理
void InitEnemy(int idx);
//　エネミー更新処理
void UpdateEnemy();
//　エネミー描画処理
void DrawEnemy();
//　エネミー終了処理
void ExitEnemy();

//　エネミーのidx番目の位置取得
DirectX::XMFLOAT3 GetEnemyPosition(int idx);
//　エネミーのidx番目の回転取得
DirectX::XMFLOAT3 GetEnemyRotation(int idx);
//　エネミーのidx番目の回転目標を取得
DirectX::XMFLOAT3 GetEnemyDestrot(int idx);
//　エネミーのidx番目のマトリックス取得
DirectX::SimpleMath::Matrix GetMtxEnemy(int idx);
//　エネミーのidx番目のOBB取得
BoundingBoxOBB GetEnemyOBB(int idx);

//　エネミーステータスを取得
int GetStsEnemy(int idx);
//　エネミータイプ取得
int GetEnemyType(int idx);
//　鍵を持っているかチェック
bool GetKeyEnemy(int idx);
//　アイテムを持っているかチェック
bool GetItemEnemy(int idx);

//　エネミーをプレイヤーに向ける
void EnemyLookPlayer();
//　エネミーのHPを減らす
void EnemyHPReduce(int idx,int dame);
//　エネミーノックバック
void EnemyKnockBack(int bulletucnt,int idx);
//　エネミー落下
void EnemyFall();

//　エネミー死亡チェック
void EnemyDeat(int idx, int bidx);
//　エネミー死亡演出
void EnemyDeathAnime();




