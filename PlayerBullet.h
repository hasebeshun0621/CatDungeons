#pragma once
#include	<directxmath.h>
#include	"renderer.h"
#include	"CPlane.h"
#include	"CMeshRenderer.h"
#include	"CBoxMesh.h"
#include	"CBoundingBox.h"
#include	"Collision.h"

#include	"Score.h"
#include	"Player.h"
#include	"CatchPointer.h"
#include	"Field.h"

//　プレイヤーバレット最大数
constexpr int PlayerBullet_Max = 100;
//　プレイヤーバレットモデル最大数
constexpr int PlayerBulletMod_Max = 3;

//　プレイヤーバレット構造体
struct PlayerBullet
{
	int sts = -1;	//未使用-1　使用中１

	int BulletType = 0;//自傷＝0　スライム弾=１スライム緑弾　＝ 2　

	DirectX::XMFLOAT3 position = { 0,0,0 };			//位置
	DirectX::XMFLOAT3 rotation = { 0,0,0 };			//回転
	DirectX::XMFLOAT3 scale = { 1,1,1 };			//サイズ

	float SpeedBullet = 0.0f;						//弾速
	DirectX::XMFLOAT3 vector = { 0,0,0 };			//バレットベクトル

	int Dame = 0;									//バレットダメージ

};

//　プレイヤーバレット初期処理
void InitPlayerBullet();
//　プレイヤーバレット更新処理
void UpdatePlayerBullet();
//　プレイヤーバレット描画処理
void DrawPlayerBullet();
//　プレイヤーバレット終了処理
void ExitPlayerBullet();

//　プレイヤーバレット位置取得
DirectX::XMFLOAT3 GetPositionPlayerBullet(int idx);
//  バレットマトリックス取得
DirectX::SimpleMath::Matrix GetMtxPlayerBullet(int idx);
//　プレイヤーバレットOBB取得
BoundingBoxOBB GetPlayerBulletOBB(int idx);

//　弾を撃てる条件チェック
bool CheckFirePlayerBullet(int idx, int Type);
//　バレット発射処理
void FirePlayerBullet(int Type);
//　プレイヤーバレット移動
void MovePlayerBullet();
//　プレイヤーバレットダメージ取得
int GetPlayerBulletDame(int idx);

//　プレイヤーバレットとエネミーの当たり判定
bool HitCheckEnemy(int idx);
//　プレイヤーバレットとボスの当たり判定
bool HitCheckBoss();
