#pragma once

#include	<directxmath.h>
#include	"CIndexBuffer.h"
#include	"CVertexBuffer.h"
#include	"CMeshRenderer.h"
#include	"scene.h"

//　タイトル用エネミー構造体
struct TitleEnemy
{
	//　描画するエネミータイプ
	int enetype = 0;

	// RTS情報
	DirectX::XMFLOAT3 position = { -270.0f,0.0f,0.0f };		// 位置
	DirectX::XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };		// 姿勢
	DirectX::XMFLOAT3 scale = { 15.0f,15.0f,15.0f };		// 拡縮
	
};

//　タイトル用エネミー初期処理
void InitTitleEnemy();
//　タイトル用エネミー更新処理
void UpdateTitleEnemy();
//　タイトル用エネミー描画処理
void DrawTitleEnemy();
//　タイトル用エネミー終了処理
void ExitTitleEnemy();

