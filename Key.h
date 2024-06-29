#pragma once
#include	<directxmath.h>
#include	"renderer.h"
#include	"CPlane.h"
#include	"CMeshRenderer.h"
#include	"CBoxMesh.h"
#include	"CBoundingBox.h"

#include	"Enemy.h"

//　カギ構造体
struct Key{

	int sts = -1;//-1透明  1ドロップ

	DirectX::SimpleMath::Vector3 position = { 0,0,0 };			//位置
	DirectX::SimpleMath::Vector3 rotation = { 0,0,0 };			//回転
	DirectX::SimpleMath::Vector3 scale = { 10,14,10 };			//スケール

};

//　カギ初期処理
void InitKey();
//　カギ更新処理
void UpdateKey();
//　カギ描画処理
void DrawKey();
//　カギ終了処理
void ExitKey();

//　カギマトリックス取得
DirectX::SimpleMath::Matrix GetMtxKey();

//　カギドロップ
void KeyDrop(int idx);
//　カギ落下ドロップ
void FallKeyDrop(int idx);
//　プレイヤーとカギの当たり判定
bool HitCheckKey();

