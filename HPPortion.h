#pragma once
#include	<directxmath.h>
#include	"renderer.h"
#include	"CPlane.h"
#include	"CMeshRenderer.h"
#include	"CBoxMesh.h"
#include	"CBoundingBox.h"

#include	"Enemy.h"

//　HPポーション最大数
constexpr int HPPortion_Max = 2;

//　HPポーション構造体
struct HPPortion {
	
	int sts = -1;											//-1透明  1ドロップ

	DirectX::SimpleMath::Vector3 position = { 0,0,0 };		//位置
	DirectX::SimpleMath::Vector3 rotation = { 0,0,0 };		//回転
	DirectX::SimpleMath::Vector3 scale = { 10,14,10 };		//スケール

	float num = 0;

};

//　HPポーション初期処理
void InitHPPortion();
//　HPポーション更新処理
void UpdateHPPortion();
//　HPポーション描画処理
void DrawHPPortion();
//　HPポーション終了処理
void ExitHPPortion();

//　HPポーションマトリックス取得
DirectX::SimpleMath::Matrix GetMtxHPPortion(int idx);
//　HPポーションOBB取得
BoundingBoxOBB GetHPPortionOBB(int idx);

//　HPポーションドロップ
void HPPortionDrop(int idx);

//　HPポーション落下ドロップ
void FallHPPotionDrop(int idx);

//　プレイヤーとHPポーションの当たり判定
void HitCheckHPPortion();

