#include "HPPortion.h"

using namespace DirectX::SimpleMath;

//　HPポーション構造体
HPPortion g_HPPortion[HPPortion_Max];

// シェーダー
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

//　HPポーションマトリックス取得
DirectX::SimpleMath::Matrix GetMtxHPPortion(int idx) {

	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_HPPortion[idx].scale,
		g_HPPortion[idx].rotation,
		g_HPPortion[idx].position);

	return mtx;
}

//　HPポーションOBB取得
BoundingBoxOBB GetHPPortionOBB(int idx) {
	Matrix mtx = GetMtxHPPortion(idx);
	return g_obb.MakeOBB(mtx, g_HPPortion[idx].scale);
}

//　HPポーション初期処理
void InitHPPortion() {

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// 頂点シェーダ
		"shader/vertexLightingPS.hlsl");				// ピクセルシェーダ

	// モデルファイル名
	std::string filename[] = {
		u8"assets\\model\\gposhonmini.obj"
	};

	for (int i = 0; i < HPPortion_Max; i++) {
		//初期化
		g_HPPortion[i].sts = -1;
		g_HPPortion[i].num = 0.5f;
	}

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

//　HPポーション更新処理
void UpdateHPPortion() {

	for (int i = 0; i < HPPortion_Max; i++) {
		//ポーションアニメーション
		if (g_HPPortion[i].sts == 1) {
			if (g_HPPortion[i].position.y < 0 || g_HPPortion[i].position.y > 20) {
				g_HPPortion[i].num *= -1;
			}

			g_HPPortion[i].position.y += g_HPPortion[i].num;
			g_HPPortion[i].rotation.y += 0.05f;

			if (g_HPPortion[i].position.y > 0) {
				g_HPPortion[i].position.y -= 2;
			}
		}
	}

	HitCheckHPPortion();
}

//　HPポーション描画処理
void DrawHPPortion() {

	for (int i = 0; i < HPPortion_Max; i++) {
		if (g_HPPortion[i].sts == 1) {
			// デバイスコンテキスト取得
			ID3D11DeviceContext* devicecontext;
			devicecontext = Renderer::GetDeviceContext();

			// ワールド変換行列生成
			Matrix mtx;
			DX11MakeWorldMatrixRadian(
				mtx,
				g_HPPortion[i].scale,							// スケール
				g_HPPortion[i].rotation,							// 姿勢
				g_HPPortion[i].position);						// 位置

			// GPUに行列をセットする
			Renderer::SetWorldMatrix(&mtx);

			// シェーダーをGPUにセット
			g_shader.SetGPU();

			// モデル描画
			g_staticmeshrenderer.Draw();
			//-------------------------------------//球描画
			// 境界ボックス描画
			//mtx = g_obb.MakeWorldMtx(g_HPPortion.scale, mtx);
			//Renderer::SetWorldMatrix(&mtx);
			//g_material.SetGPU();
			//g_meshrenderer.Draw();
		}
	}

}

//　HPポーション終了処理
void ExitHPPortion() {
	g_staticmeshrenderer.Uninit();
}

//HPポーションドロップ
void HPPortionDrop(int idx) {
	
	for (int i = 0; i < HPPortion_Max; i++) {
		if (g_HPPortion[i].sts <= -1) {
			//エネミーが死んでおり鍵を持っているなら
			if (GetItemEnemy(idx) && GetStsEnemy(idx) == -1) {
				g_HPPortion[i].position = GetEnemyPosition(idx);
				g_HPPortion[i].sts = 1;
				break;
			}
		}
	}

}

//　HPポーション落下ドロップ
void FallHPPotionDrop(int idx) {

	for (int i = 0; i < HPPortion_Max; i++) {
		if (g_HPPortion[i].sts <= -1) {
			//エネミーが死んでおり鍵を持っているなら
			if (GetItemEnemy(idx) && GetStsEnemy(idx) == -1) {
				g_HPPortion[i].position.x = (rand() % (int)FieldX) - FieldX / 2;
				g_HPPortion[i].position.y = 400.0f;
				g_HPPortion[i].position.z = (rand() % (int)FieldZ) - FieldZ / 2;;
				g_HPPortion[i].sts = 1;
			}
			break;
		}
	}

}

//　プレイヤーとHPポーションの当たり判定
void HitCheckHPPortion() {
	
	for (int i = 0; i < HPPortion_Max; i++) {
		if (g_HPPortion[i].sts == 1) {
			if (HitCheckOBB(GetPlayerOBB(), GetHPPortionOBB(i))) {
				if (NowBagCnt() >= BagMax) {
					g_HPPortion[i].sts = -1;
				}
				else
				{
					g_HPPortion[i].sts = -1;
					HPPortionCntUp();
					PlaySound(GetItemSE);
				}
			}
			break;
		}
	}

}