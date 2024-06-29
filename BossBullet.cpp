#include	"BossBullet.h"

using namespace DirectX::SimpleMath;

//　ボスバレット構造体
BossBullet g_bossbullet[BulletEnemy_Max];

//ノックバック、オンオフ
static bool kbsts = false;

// シェーダー
static CShader		g_shader;

// スタティックメッシュ（ジオメトリデータ）
static CStaticMesh g_staticmesh;

// メッシュレンダラー
static CStaticMeshRenderer g_staticmeshrenderer;

// メッシュレンダラ
static CMeshRenderer g_meshrenderer;

// マテリアル
static CMaterial g_material;

// ボックスメッシュ
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb;

// ボスバレットOBB取得
BoundingBoxOBB GetBossBulletOBB(int idx) {
	Matrix mtx = GetMtxBossBullet(idx);
	return g_obb.MakeOBB(mtx, g_bossbullet[idx].scale);
}

//　ボスバレットダメージ取得
int GetBossBulletDame(int idx) {
	return g_bossbullet[idx].Dame;
}

//　ボスバレットマトリックス取得
DirectX::SimpleMath::Matrix GetMtxBossBullet(int idx) {

	Matrix bmtx;
	DX11MakeWorldMatrixRadian(
		bmtx,
		g_bossbullet[idx].scale,
		g_bossbullet[idx].rotation,
		g_bossbullet[idx].position);

	return bmtx;
}

//　ボスバレット初期化
BossBullet InitializBossBullet() {

	BossBullet obj;
	obj.sts = -1;								//未使用-1　使用中１
	obj.position = { 0,0,0 };					//位置
	obj.rotation = { 0,0,0 };					//回転
	obj.scale = { 9.0f,11.0f,10.0f };			//サイズ
	obj.vector = { 0.0f,0.0f,0.0f };			//ベクトル取得
	obj.Speed = 4.0f;							//弾速
	obj.Dame = ENEMYBULLETDAME_BOSS;

	return obj;
}

//　ボスバレット初期処理
void InitBossBullet() {
	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// 頂点シェーダ
		"shader/vertexLightingPS.hlsl");				// ピクセルシェーダ

	// モデルファイル名
	std::string filename[] = {
		u8"assets\\model\\ya.obj",
	};

	//ノックバック、オンオフ
	kbsts = false;

	// メッシュ生成（ジオメトリデータ）
	g_staticmesh.Init(filename[0]);

	// 描画の為のデータ生成
	g_staticmeshrenderer.Init(g_staticmesh);

	for (int i = 0; i < BossBullet_Max; i++) {
		//エネミー初期化
		g_bossbullet[i] = InitializBossBullet();
	}

	//ーーーーーーーーーーーーーーーーーーーー//
	// OBBを求める
	g_obb.Caliculate(g_staticmesh.GetVertices());

	// 箱メッシュを生成する
	Vector3 diff = g_obb.GetMax() - g_obb.GetMin();
	g_box.Init(
		diff.x,
		diff.y,
		diff.z,
		Color(1, 1, 1, 1));
	g_meshrenderer.Init(g_box);

	// OBB描画用マテリアル
	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 0, 0.3f);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.TextureEnable = FALSE;
	g_material.Init(mtrl);

}

//　ボスバレット更新処理
void UpdateBossBullet() {

	MoveBossBullet();
	HitCheckPlayer_Boss();

}

//　ボスバレット描画処理
void DrawBossBullet() {
	
	// デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	for (int i = 0; i < BossBullet_Max; i++) {
		if (g_bossbullet[i].sts == 1) {
			// ワールド変換行列生成
			Matrix mtx;
			DX11MakeWorldMatrixRadian(
				mtx,
				g_bossbullet[i].scale,							// スケール
				g_bossbullet[i].rotation,						// 姿勢
				g_bossbullet[i].position);						// 位置

			// GPUに行列をセットする
			Renderer::SetWorldMatrix(&mtx);

			// シェーダーをGPUにセット
			g_shader.SetGPU();
			// モデル描画
			g_staticmeshrenderer.Draw();

			//-------------------------------------//球描画
			////OBB情報生成
			////境界ボックス描画
			//mtx = g_obb[g_enemybullet[i].BulletType].MakeWorldMtx(g_enemybullet[i].scale, mtx);
			//Renderer::SetWorldMatrix(&mtx);
			//// マテリアルをセット
			//g_material.SetGPU();
			//g_meshrenderer[g_enemybullet[i].BulletType].Draw();
		}
	}

}

//　ボスバレット終了処理
void ExitBossBullet() {

	g_staticmeshrenderer.Uninit();

}

//　ボスバレット発射処理
void FireBossBullet(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot) {

	//プレイヤーポジション取得
	DirectX::XMFLOAT3 playerpos = GetPositionPlayer();

	for (int i = 0; i < BossBullet_Max; i++) {
		//使用済みバレット回収
		if (g_bossbullet[i].sts == 1) {

		}

		//未使用バレット取得->使用
		if (g_bossbullet[i].sts == -1) {
			//差の斜辺を求める
			//差のｘを斜辺で割る、Ｙも
			//求められるのはベクトルを１で表した数これをnormalizeと言う

			//バレットがエネミーの座標を取得
			g_bossbullet[i].position = pos;

			//エネミーのI番目の発射方向取得
			g_bossbullet[i].rotation = rot;

			//プレイヤーとエネミーの距離(差)
			DirectX::XMFLOAT3 sa = playerpos - g_bossbullet[i].position;

			//ベクトルの長さ
			float v = sqrt((sa.x * sa.x) + (sa.z * sa.z));

			//単位ベクトル
			g_bossbullet[i].vector = DirectX::XMFLOAT3(sa.x / v, sa.y, sa.z / v);

			g_bossbullet[i].sts = 1;
			break;//未使用が見つかればforから出る
		}
	}

}

//　ボスバレット移動
void MoveBossBullet() {

	for (int i = 0; i < BossBullet_Max; i++) {
		if (g_bossbullet[i].sts == 1) {
			//移動更新
			g_bossbullet[i].position.x += g_bossbullet[i].vector.x * g_bossbullet[i].Speed;
			g_bossbullet[i].position.z += g_bossbullet[i].vector.z * g_bossbullet[i].Speed;

		}

		if (MapOutCheck(g_bossbullet[i].position)) {
			g_bossbullet[i].sts = -1;

		}
	}

}

//　ボスバレットとプレイヤーの当たり判定
void HitCheckPlayer_Boss() {

	static	int ebnum = 0;
	if (GetPlayerHitSts()) {
		for (int i = 0; i < BossBullet_Max; i++) {
			bool sts = HitCheckOBB(GetPlayerOBB(), GetBossBulletOBB(i));
			if (sts && g_bossbullet[i].sts == 1) {
				g_bossbullet[i].sts = -1;
				//弾のダメージ分プレイヤーの体力を減らす
				PlayerHPDOWN(GetBossBulletDame(i));
				MakeDameParticle(g_bossbullet[i].position);
				SetHitStop(5);

				ebnum = i;
				kbsts = true;
			}
		}
	}
	if (kbsts && HitStop()) {
		kbsts = PlayerKnockBack(ebnum,2);
	}

}
