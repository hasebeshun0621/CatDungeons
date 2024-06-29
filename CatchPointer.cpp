#include	"CatchPointer.h"

using namespace DirectX::SimpleMath;

//　ポインタ構造体
Pointer g_pointer;

// シェーダー
static CShader		g_shader;

// スタティックメッシュ（ジオメトリデータ）
static CStaticMesh g_staticmesh[Model_Max];

// メッシュレンダラー
static CStaticMeshRenderer g_staticmeshrenderer[Model_Max];

//ーーーーーーーーーーーーーーーーーーーーー//
//obbメッシュ、シェーダ定義

// メッシュレンダラ
static CMeshRenderer g_meshrenderer;

// マテリアル
static CMaterial g_material;

// ボックスメッシュ
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb;

//　ポインタマトリックス取得
DirectX::SimpleMath::Matrix GetMtxPointer() {

	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_pointer.scale,
		g_pointer.rotation,
		g_pointer.position);

	return mtx;
}

// OBB取得
BoundingBoxOBB GetPointerOBB() {
	return g_obb.MakeOBB(GetMtxPointer(), g_pointer.scale);
}

//　ポインタステータスを取得
int GetStsPointer() {
	return g_pointer.sts;
}

//　バレットキャッチ合計数取得
int GetCatchCnt() {
	g_pointer.CatchCum = g_pointer.CatchBullet_Sura + g_pointer.CatchBullet_SuraG;
	return g_pointer.CatchCum;
}

//　タイプ別キャッチ数取得
int GetCatchBullet(int Type) {

	switch (Type)
	{
	case 0:
		return	999;
		break;
	case 1:
		return	g_pointer.CatchBullet_Sura;
		break;
	case 2:
		return	g_pointer.CatchBullet_SuraG;
		break;
	}

}

//　バレット数を増やす
void CatchCntUp(int Type) {

	switch (Type)
	{
	case 0:
		g_pointer.CatchBullet_Sura++;
		break;
	case 1:
		g_pointer.CatchBullet_SuraG++;
		break;
	}

}

//　バレット数を減らす
void CatchCntDown(int Type) {

	switch (Type)
	{
	case 0:
		break;
	case 1:
		g_pointer.CatchBullet_Sura--;
		break;
	case 2:
		g_pointer.CatchBullet_SuraG--;
		break;
	}

}

//　ポインタ初期化
Pointer InitializPointer() {

	Pointer obj;
	obj.sts = 0;	//矢印-1　キャッチ１

	obj.position = { 0,0,0 };			//位置
	obj.rotation = { 0,0,0 };			//回転
	obj.scale = { 12,11,12 };			//サイズ	
	obj.vector = { 0,0,0 };

	obj.CatchCum = 0;
	obj.CatchBullet_Sura = 0;
	obj.CatchBullet_SuraG = 0;

	obj.Catchfcnt = 0;
	obj.CatchReuse = 0;

	return obj;
}

//　ポインタ初期処理
void InitPointer() {

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// 頂点シェーダ
		"shader/vertexLightingPS.hlsl");				// ピクセルシェーダ

	// モデルファイル名
	std::string filename[] = {
		u8"assets\\model\\pointer.obj",
		u8"assets\\model\\net.obj"
	};

	//ポインタとネットの二個生成
	for (int i = 0; i < Model_Max; i++) {
		// メッシュ生成（ジオメトリデータ）
		g_staticmesh[i].Init(filename[i]);

		// 描画の為のデータ生成
		g_staticmeshrenderer[i].Init(g_staticmesh[i]);


	}

	g_pointer = InitializPointer();

	//ーーーーーーーーーーーーーーーーーーーー//

	// OBB生成
	g_obb.Caliculate(g_staticmesh[1].GetVertices());

	// OBB描画用メッシュ生成
	Vector3 diff = g_obb.GetMax() - g_obb.GetMin();
	g_box.Init(diff.x, diff.y, diff.z, Color(1, 1, 1, 1));
	g_meshrenderer.Init(g_box);

	// OBB描画用マテリアル
	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 0.5f);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.TextureEnable = FALSE;

	g_material.Init(mtrl);

}

//　ポインタ更新処理
void UpdatePointer() {
	
	//プレイヤー前方位置を常に取得
	g_pointer.position = GetPositionPlayer();
	g_pointer.rotation = GetRotationPlayer();

	Matrix pmtx = GetMtxPlayer();
	g_pointer.position.x -= pmtx._31 * 0.8;
	g_pointer.position.y -= pmtx._32 -  4;
	g_pointer.position.z -= pmtx._33 * 0.8;
	
	//　スペースを押すとネットを振る
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
		if (g_pointer.CatchReuse <= 0) {
			g_pointer.CatchFlag = true;
			//ネット再使用可能までのタイム設定
			g_pointer.CatchReuse = 20;
			PlaySound(NetSE);
		}
	}
	
	if(g_pointer.CatchFlag&&g_pointer.Catchfcnt > 0){
		g_pointer.Catchfcnt--;
		g_pointer.sts = NET;
	}
	else {
		g_pointer.CatchFlag = false;
		//ネット展開時間
		g_pointer.Catchfcnt = 35;
		if (g_pointer.CatchReuse >= 0) {
			g_pointer.CatchReuse--;
		}
		g_pointer.sts = POINTER;
	}	
	
	MoveNet(g_pointer.CatchFlag);

}

//　ポインタ描画処理
void DrawPointer() {

	// デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ワールド変換行列生成
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_pointer.scale,							// スケール
		g_pointer.rotation,							// 姿勢
		g_pointer.position);						// 位置g_pointermtx

	// GPUに行列をセットする
	Renderer::SetWorldMatrix(&mtx);

	// シェーダーをGPUにセット
	g_shader.SetGPU();

	// モデル描画
	g_staticmeshrenderer[g_pointer.sts].Draw();
	//-------------------------------------//球描画

	//// 境界ボックス描画
	//mtx = g_obb.MakeWorldMtx(g_pointer.scale, mtx);

	//Renderer::SetWorldMatrix(&mtx);
	//g_material.SetGPU();
	//g_meshrenderer.Draw();

}

//　ポインタ終了処理
void ExitPointer() {

	for (int i = 0; i < Model_Max; i++) {
		g_staticmeshrenderer[i].Uninit();
	}

}

//　ネットを振る
void MoveNet(bool flag) {

	static float rotcnt = 0;
	if (flag) {
		//ネット回転速度
		rotcnt+=0.09f;
		g_pointer.rotation.y -= rotcnt;
		g_pointer.rotation.x -= rotcnt/3;
	}
	else {
		rotcnt = 0;
		
	}

}


