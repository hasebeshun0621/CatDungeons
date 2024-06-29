#include	"EnemyBullet.h"

using namespace DirectX::SimpleMath;

//　エネミーバレット構造体
EnemyBullet g_enemybullet[BulletEnemy_Max];

//ノックバック、オンオフ
static bool kbsts = false;

// シェーダー
static CShader		g_shader;

// スタティックメッシュ（ジオメトリデータ）
static CStaticMesh g_staticmesh[EnemyBulletMod_Max];

// メッシュレンダラー
static CStaticMeshRenderer g_staticmeshrenderer[EnemyBulletMod_Max];

//ーーーーーーーーーーーーーーーーーーーーー//

// メッシュレンダラ
static CMeshRenderer g_meshrenderer[EnemyBulletMod_Max];

// マテリアル
static CMaterial g_material;

// ボックスメッシュ
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb[EnemyBulletMod_Max];

//　エネミーバレットマトリックス取得
DirectX::SimpleMath::Matrix GetMtxEnemyBullet(int idx) {

	Matrix bmtx;
	DX11MakeWorldMatrixRadian(
		bmtx,
		g_enemybullet[idx].scale,
		g_enemybullet[idx].rotation,
		g_enemybullet[idx].position);

	return bmtx;
}

// エネミーバレットOBB取得
BoundingBoxOBB GetEnemyBulletOBB(int idx) {
	Matrix mtx = GetMtxEnemyBullet(idx);
	return g_obb[g_enemybullet[idx].BulletType].MakeOBB(mtx, g_enemybullet[idx].scale);
}

//　エネミーバレットのタイプを取得
int GetEnemyBulletType(int idx) {
	return g_enemybullet[idx].BulletType;
}

//　エネミーバレットのスピードを取得
float GetEnemyBulletSpeed(int idx){
	return g_enemybullet[idx].Speed;
}

//　エネミーバレットダメージ取得
int GetEnemyBulletDame(int idx) {
	return g_enemybullet[idx].Dame;
}

//　エネミーバレットタイプ別初期化
EnemyBullet InitializBullet_Sura() {

	EnemyBullet obj;
	obj.sts = -1;
	obj.BulletType = 0;
	obj.position = { 0,0,0 };
	obj.rotation = { 0,0,0 };
	obj.scale = { 10.0f,10.0f,12.0f };
	obj.vector = { 0.0f,0.0f,0.0f };
	obj.Speed = 2.5f;
	obj.Dame = ENEMYBULLETDAME_Sura;

	return obj;
}

EnemyBullet InitializBullet_SuraLeaf() {

	EnemyBullet obj;
	obj.sts = -1;
	obj.BulletType = 1;
	obj.position = { 0,0,0 };
	obj.rotation = { 0,0,0 };
	obj.scale = { 9.0f,11.0f,10.0f };
	obj.vector = { 0.0f,0.0f,0.0f };
	obj.Speed = 2.0f;
	obj.Dame = ENEMYBULLETDAME_SuraLeaf;

	return obj;
}

//　エネミーバレット初期処理
void InitEnemyBullet() {

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// 頂点シェーダ
		"shader/vertexLightingPS.hlsl");				// ピクセルシェーダ

	// モデルファイル名
	std::string filename[] = {
		u8"assets\\model\\suraBullet.obj",
		u8"assets\\model\\leafBullet.obj",
	};
	
	//ノックバック、オンオフ
	kbsts = false;

	for (int i = 0; i < EnemyBulletMod_Max; i++) {
		// メッシュ生成（ジオメトリデータ）
		g_staticmesh[i].Init(filename[i]);

		// 描画の為のデータ生成
		g_staticmeshrenderer[i].Init(g_staticmesh[i]);

		// OBBを求める
		g_obb[i].Caliculate(g_staticmesh[i].GetVertices());

		// 箱メッシュを生成する
		Vector3 diff = g_obb[i].GetMax() - g_obb[i].GetMin();
		g_box.Init(
			diff.x,
			diff.y,
			diff.z,
			Color(1, 1, 1, 1));

		g_meshrenderer[i].Init(g_box);
	}

	for (int i = 0; i < BulletEnemy_Max; i++) {
		static int cnt = 0;
		switch (cnt)
		{
		case 0:
			//エネミー初期化
			g_enemybullet[i] = InitializBullet_Sura();
			cnt++;
			break;
		case 1:
			//エネミー初期化
			g_enemybullet[i] = InitializBullet_SuraLeaf();
			cnt = 0;
			break;
		}
	}
	//ーーーーーーーーーーーーーーーーーーーー//
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

//　エネミーバレット更新処理
void UpdateEnemyBullet() {

	MoveEnemyBullet();
	HitCheckPlayer_Enemy();

}

//　エネミーバレット描画処理
void DrawEnemyBullet() {

	// デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	for (int i = 0; i < BulletEnemy_Max; i++) {
		if (g_enemybullet[i].sts == 1) {
			// ワールド変換行列生成
			Matrix mtx;
			DX11MakeWorldMatrixRadian(
				mtx,
				g_enemybullet[i].scale,							// スケール
				g_enemybullet[i].rotation,						// 姿勢
				g_enemybullet[i].position);						// 位置

			// GPUに行列をセットする
			Renderer::SetWorldMatrix(&mtx);

			// シェーダーをGPUにセット
			g_shader.SetGPU();
			// モデル描画
			g_staticmeshrenderer[g_enemybullet[i].BulletType].Draw();

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

//　エネミーバレット終了処理
void ExitEnemyBullet() {

	for (int i = 0; i < EnemyBulletMod_Max; i++) {
		g_staticmeshrenderer[i].Uninit();
	}

}

//　エネミーバレット発射処理
void FireEnemyBullet(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT3 rot,int type) {
	
	//プレイヤーポジション取得
	DirectX::XMFLOAT3 playerpos = GetPositionPlayer();

	for (int i = 0; i < BulletEnemy_Max; i++) {
		//エネミーのタイプと弾のタイプが一致している弾を取得
		if (type == g_enemybullet[i].BulletType) {
			//使用済みバレット回収
			if (g_enemybullet[i].sts == 1) {

			}

			//未使用バレット取得->使用
			if (g_enemybullet[i].sts == -1) {
				//差の斜辺を求める
				//差のｘを斜辺で割る、Ｙも
				//求められるのはベクトルを１で表した数これをnormalizeと言う

				//バレットがエネミーの座標を取得
				g_enemybullet[i].position = pos;

				//エネミーのI番目の発射方向取得
				g_enemybullet[i].rotation = rot;

				//プレイヤーとエネミーの距離(差)
				DirectX::XMFLOAT3 sa = playerpos - g_enemybullet[i].position;

				//ベクトルの長さ
				float v = sqrt((sa.x * sa.x) + (sa.z * sa.z));

				//単位ベクトル
				g_enemybullet[i].vector = DirectX::XMFLOAT3(sa.x / v, sa.y, sa.z / v);

				g_enemybullet[i].sts = 1;
				break;//未使用が見つかればforから出る
			}
		}
	}

}

//　エネミーバレット移動
void MoveEnemyBullet() {

	for (int i = 0; i < BulletEnemy_Max; i++) {
		if (g_enemybullet[i].sts == 1) {
			//移動更新
			g_enemybullet[i].position.x += g_enemybullet[i].vector.x * g_enemybullet[i].Speed;
			g_enemybullet[i].position.z += g_enemybullet[i].vector.z * g_enemybullet[i].Speed;
		}

		if (MapOutCheck(g_enemybullet[i].position)) {
			g_enemybullet[i].sts = -1;
		}
	}

}

//　プレイヤーとエネミーバレットの当たり判定
void HitCheckPlayer_Enemy() {
	
	static	int ebnum = 0;
	if (GetPlayerHitSts()) {
		for (int i = 0; i < BulletEnemy_Max; i++) {
			bool sts = HitCheckOBB(GetPlayerOBB(), GetEnemyBulletOBB(i));
			if (sts && g_enemybullet[i].sts == 1) {
				g_enemybullet[i].sts = -1;
				//弾のダメージ分プレイヤーの体力を減らす
				PlayerHPDOWN(GetEnemyBulletDame(i));
				SetHitStop(4);
				MakeDameParticle(g_enemybullet[i].position);
				ebnum = i;
				kbsts = true;
			}
		}
	}
	if (kbsts&&HitStop()) {
		kbsts = PlayerKnockBack(ebnum,1);
	}

}

//　バレットとネットの当たり判定
bool HitCheckNet() {
	
	for (int i = 0; i < BulletEnemy_Max; i++) {
		bool sts = HitCheckOBB(GetPointerOBB(),GetEnemyBulletOBB(i));
		//ヒット中にボタンを押したなら
		if (sts && g_enemybullet[i].sts == 1 && GetStsPointer() == 1) {
			g_enemybullet[i].sts = -1;

			CatchCntUp(g_enemybullet[i].BulletType);

			SetHitStop(10);//ヒットストップ設定

			MakeCatchParticle(g_enemybullet[i].position);
			MakeBulletCatchParticle(g_enemybullet[i].position);

			PlaySound(CatchSE);

			return true;
		}
	}

	return false;
}