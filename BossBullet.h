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

//　ボスバレット最大数
constexpr int BossBullet_Max = 200;
//　ボスバレットダメージ
constexpr int ENEMYBULLETDAME_BOSS = 15;

//　ボスバレット構造体
struct BossBullet
{

	int sts = -1;									//未使用-1　使用中１

	DirectX::XMFLOAT3 position = { 0,0,0 };			//位置
	DirectX::XMFLOAT3 rotation = { 0,0,0 };			//回転
	DirectX::XMFLOAT3 scale = { 1,1,1 };			//サイズ
	DirectX::XMFLOAT3 vector = { 0,0,0 };			//単位ベクトル

	float Speed = 0.0f;								//弾速
	int Dame = 0;									//ダメージ

};

//　ボスバレット初期化
BossBullet InitializBossBullet();

//　ボスバレット初期処理
void InitBossBullet();
//　ボスバレット更新処理
void UpdateBossBullet();
//　ボスバレット描画処理
void DrawBossBullet();
//　ボスバレット終了処理
void ExitBossBullet();

//　ボスバレットマトリックス取得
DirectX::SimpleMath::Matrix GetMtxBossBullet(int idx);
//　ボスバレットダメージ取得
int GetBossBulletDame(int idx);

//　ボスバレットとプレイヤーの当たり判定
void HitCheckPlayer_Boss();

//　ボスバレット発射処理
void FireBossBullet(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot);

//　ボスバレット移動
void MoveBossBullet();

