#pragma once
#include	<directxmath.h>
#include	"renderer.h"
#include	"CPlane.h"
#include	"CMeshRenderer.h"
#include	"CBoxMesh.h"
#include	"CBoundingBox.h"

#include	"WaveScene.h"

#include	"Collision.h"

#include	"CatchPointer.h"
#include	"Enemy.h"

#include	"Field.h"

//　エネミーバレット最大数
constexpr int BulletEnemy_Max=200;

//　エネミーバレットモデル最大数
constexpr int EnemyBulletMod_Max = 2;

//　エネミー別バレットダメージ
constexpr int ENEMYBULLETDAME_Sura = 15;
constexpr int ENEMYBULLETDAME_SuraLeaf = 10;

//　エネミーバレット構造体
struct EnemyBullet
{
	int sts = -1;									//未使用-1　使用中１

	int BulletType = 0;								//青スライム＝０　緑スライム＝1

	DirectX::XMFLOAT3 position = { 0,0,0 };			//位置
	DirectX::XMFLOAT3 rotation = { 0,0,0 };			//回転
	DirectX::XMFLOAT3 scale = { 1,1,1 };			//サイズ
	DirectX::XMFLOAT3 vector = { 0,0,0 };			//単位ベクトル

	float Speed = 0.0f;								//弾速
	int Dame = 0;									//ダメージ

};

//　エネミーバレット初期処理
void InitEnemyBullet();
//　エネミーバレット更新処理
void UpdateEnemyBullet();
//　エネミーバレット描画処理
void DrawEnemyBullet();
//　エネミーバレット終了処理
void ExitEnemyBullet();

//　エネミーバレットマトリックス取得
DirectX::SimpleMath::Matrix GetMtxEnemyBullet(int idx);

//　エネミーバレットのタイプを取得
int GetEnemyBulletType(int idx);
//　エネミーバレットスピード取得
float GetEnemyBulletSpeed(int idx);
//　エネミーバレットダメージ取得
int GetEnemyBulletDame(int idx);

//　バレットとネットの当たり判定
bool HitCheckNet();
//　プレイヤーとエネミーバレットの当たり判定
void HitCheckPlayer_Enemy();
//　エネミーバレット発射処理
void FireEnemyBullet(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot,int type);
//　エネミーバレット移動
void MoveEnemyBullet();
