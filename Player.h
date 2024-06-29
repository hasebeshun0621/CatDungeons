#pragma once
#include	<directxmath.h>
#include	"renderer.h"
#include	"CPlane.h"
#include	"CMeshRenderer.h"
#include	"CBoxMesh.h"
#include	"CBoundingBox.h"

#include	"HPPortion.h"
#include	"scene.h"
#include	"HPUI.h"
#include	"Field.h"
#include	"BodyParticle.h"

//　プレイヤー最大HP
constexpr int PlayerHP_Max = 100;
// プレイヤーがアビリティを使うと減る体力
constexpr int PlayerAbilityDame = 30;
//　プレイヤーのアイテム所持最大数
constexpr int BagMax = 5;

enum ItemName
{
	ABILITY=0,	//アビリティ
	HEAL,		//回復
	BULLET,		//バレット
};

//　プレイヤー構造体
struct Player {

	// プレイヤの情報(S,R,T)
	DirectX::SimpleMath::Vector3 position = { 0.0f,0.0f,0.0f };		// 位置
	DirectX::SimpleMath::Vector3 rotation = { 0.0f,0.0f,0.0f };		// 姿勢
	DirectX::SimpleMath::Vector3 scale = { 15.0f,15.0f,15.0f };		// 拡縮

	float VALUE_MOVE_MODEL = 0.0f;						// 移動スピード
	float VALUE_ROTATE_MODEL = DirectX::XM_PI * 0.0f;	// 回転スピード
	float RATE_ROTATE_MODEL = 0.0f;						// 回転慣性係数
	float RATE_MOVE_MODEL = 0.0f;						// 移動慣性係数

	bool HitSts = true;		//当たり判定　falseなし　trueあり
	bool Control = true;	//プレイヤー操作　falseなし trueあり
	bool EvasionSwitch = false;// 回避オンオフ

	int ItemPointer = 0;	//選択している十字キー
	int ItemSideCnt = 0;	//横　選んでいるアイテム番号

	int HPPortionCnt = 0;	//所持しているポーション数

	int HP = 0;				//体力
	int ST = 0;				//スタミナ
	int STcnt = 0;			//スタミナ回復速度

};

//　プレイヤー初期処理
void InitPlayer();
//　プレイヤー更新処理
void UpdatePlayer();
//　プレイヤー描画処理
void DrawPlayer();
//　プレイヤー終了処理
void ExitPlayer();

//　プレイヤー操作まとめ
void PlayerControl();
//　プレイヤーダッシュ
void PlayerDash();
//　プレイヤー回避
void PlayerEvasion();
//　プレイヤー落下
bool PlayerFall();
//　プレイヤーポジションリセット
void ResetPosPlayer();
//　プレイヤーノックバック
bool PlayerKnockBack(int idx,int type);//type　1エネミー、2ボス

//プレイヤー初期化
Player initializePlayer();

// 位置取得
DirectX::XMFLOAT3 GetPositionPlayer();
// 姿勢取得
DirectX::XMFLOAT3 GetRotationPlayer();
//サイズ取得
DirectX::XMFLOAT3 GetScalePlayer();
// プレイヤーマトリクス取得
DirectX::SimpleMath::Matrix GetMtxPlayer();
// OBB取得
BoundingBoxOBB GetPlayerOBB();
// 移動量取得
DirectX::XMFLOAT3 GetMovePlayer();
// 回転目標角度取得
DirectX::XMFLOAT3 GetDestrotPlayer();

bool GetPlayerHitSts();
//プレイヤーHPゲット
int GetPlayerHp();
//　HPを増やす
void PlayerHPUP(int num);
//　HPを減らす
void PlayerHPDOWN(int num);

//プレイヤーSTゲット
int GetPlayerSt();
//　プレイヤースタミナを減らす
void PlayerSTDOWN(int num);

//　HPポーション所持数
int GetHPPortionCnt();
//　HPポーションを増やす
void HPPortionCntUp();
//　HPポーションを減らす
void HPPortionCntDown();

//　今選んでいるアイテム欄
int GetItemPointer();
//　今選んでいるバレット
int GetSelectBullet();

