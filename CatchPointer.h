#pragma once
#include	<directxmath.h>
#include	"renderer.h"
#include	"CPlane.h"
#include	"CMeshRenderer.h"

#include	"Player.h"
#include	"PlayerBullet.h"
#include	"EnemyBullet.h"
#include	"Field.h"

enum PointerSts {
	POINTER,	//ポインタ
	NET,		//ネット
	Model_Max,	//モデル最大数
};

//　ポインタ構造体
struct Pointer{

	int sts =POINTER;							//矢印 0　キャッチ１

	DirectX::XMFLOAT3 position = { 0,0,0 };		//位置
	DirectX::XMFLOAT3 rotation = { 0,0,0 };		//回転
	DirectX::XMFLOAT3 scale	   = { 1,1,1 };		//サイズ

	DirectX::XMFLOAT3 vector = { 0,0,0 };		//単位ベクトル

	int CatchCum = 0;							//弾を拾った合計数
	int CatchBullet_Sura = 0;					//sura弾数
	int CatchBullet_SuraG = 0;					//suragreen弾数

	bool CatchFlag = false;						//ネットオンオフ
	int Catchfcnt = 0;							//ネットを広げれるタイム
	int CatchReuse = 0;							//ネット再使用タイム

};

//　ポインタ初期処理
void InitPointer();
//　ポインタ更新処理
void UpdatePointer();
//　ポインタ描画処理
void DrawPointer();
//　ポインタ終了処理
void ExitPointer();

//　ポインタOBB取得
BoundingBoxOBB GetPointerOBB();

//　ポインタステータスを取得
int GetStsPointer();
//　バレットキャッチ合計数取得
int GetCatchCnt();
//　タイプ別キャッチ数取得
int GetCatchBullet(int Type);
//　バレット数を増やす
void CatchCntUp(int Type);
//　バレット数を減らす
void CatchCntDown(int Type);

//　ネットを振る
void MoveNet(bool flag);
