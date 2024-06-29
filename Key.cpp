#include "Key.h"

using namespace DirectX::SimpleMath;

//　カギ構造体
Key g_Key;

// 使用するシェーダー
static CShader		g_shader;

// スタティックメッシュ（ジオメトリデータ）
static CStaticMesh g_staticmesh;

// メッシュレンダラー
static CStaticMeshRenderer g_staticmeshrenderer;

//---------------------------------------------------//
//obbメッシュ、シェーダ定義

// メッシュレンダラ
static CMeshRenderer g_meshrenderer;

// マテリアル
static CMaterial g_material;

// ボックスメッシュ
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb;

//　カギステータス取得
int GetKeySts() {
	return g_Key.sts;
}

//　カギマトリックス取得
DirectX::SimpleMath::Matrix GetMtxKey() {

	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_Key.scale,
		g_Key.rotation,
		g_Key.position);

	return mtx;
}

//　カギOBB取得
BoundingBoxOBB GetKeyOBB() {
	Matrix mtx = GetMtxKey();
	return g_obb.MakeOBB(mtx, g_Key.scale);
}

//　カギ初期処理
void InitKey() {

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// 頂点シェーダ
		"shader/vertexLightingPS.hlsl");				// ピクセルシェーダ

	// モデルファイル名
	std::string filename[] = {
		u8"assets\\model\\Key.obj"
	};

	g_Key.sts = -1;

	// メッシュ生成（ジオメトリデータ）
	g_staticmesh.Init(filename[0]);

	// 描画の為のデータ生成
	g_staticmeshrenderer.Init(g_staticmesh);

	//ーーーーーーーーーーーーーーーーーーーー//
	// OBB生成
	g_obb.Caliculate(g_staticmesh.GetVertices());

	// OBB描画用メッシュ生成
	Vector3 diff = g_obb.GetMax() - g_obb.GetMin();
	g_box.Init(diff.x, diff.y, diff.z, Color(1, 1, 1, 1));
	g_meshrenderer.Init(g_box);

	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 0.3f);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.TextureEnable = FALSE;
	g_material.Init(mtrl);

}

//　カギ更新処理
void UpdateKey() {

	if (g_Key.sts == 1) {
		static float num = 0.5f;
		if (g_Key.position.y < 0 || g_Key.position.y > 20) {
			num *= -1;
		}
		g_Key.position.y += num;

		if (g_Key.position.y > 0) {
			g_Key.position.y-=2;
		}
		g_Key.rotation.y += 0.05f;
	}

}

//　カギ描画処理
void DrawKey() {
	
	if (g_Key.sts == 1) {
		// デバイスコンテキスト取得
		ID3D11DeviceContext* devicecontext;
		devicecontext = Renderer::GetDeviceContext();

		// ワールド変換行列生成
		Matrix mtx;
		DX11MakeWorldMatrixRadian(
			mtx,
			g_Key.scale,							// スケール
			g_Key.rotation,							// 姿勢
			g_Key.position);						// 位置

		// GPUに行列をセットする
		Renderer::SetWorldMatrix(&mtx);

		// シェーダーをGPUにセット
		g_shader.SetGPU();

		// モデル描画
		g_staticmeshrenderer.Draw();
		//-------------------------------------//球描画
		// 境界ボックス描画
		//mtx = g_obb.MakeWorldMtx(g_Key.scale, mtx);
		//Renderer::SetWorldMatrix(&mtx);
		//g_material.SetGPU();
		//g_meshrenderer.Draw();
	}

}

//　カギ終了処理
void ExitKey() {
	g_staticmeshrenderer.Uninit();
}

//カギドロップ
void KeyDrop(int idx) {

	//エネミーが死んでおり鍵を持っているなら
	if (GetKeyEnemy(idx) &&GetStsEnemy(idx) == -1) {
		g_Key.position = GetEnemyPosition(idx);
		g_Key.sts = 1;
	}

}

//　カギ落下ドロップ
void FallKeyDrop(int idx) {

	//エネミーが死んでおり鍵を持っているなら
	if (GetKeyEnemy(idx) && GetStsEnemy(idx) == -1) {
		g_Key.position.x = (rand() % (int)FieldX) - FieldX / 2;
		g_Key.position.y = 400.0f;
		g_Key.position.z = (rand() % (int)FieldZ) - FieldZ / 2;;
		g_Key.sts = 1;
	}

}

//　プレイヤーとカギの当たり判定
bool HitCheckKey() {

	bool sts = false;
	if (g_Key.sts == 1) {
		sts = HitCheckOBB(GetPlayerOBB(), GetKeyOBB());
	}

	return sts;
}


