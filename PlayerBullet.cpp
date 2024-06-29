#include	"PlayerBullet.h"

using namespace DirectX::SimpleMath;

//　プレイヤーバレット構造体
PlayerBullet g_playerbullet[PlayerBullet_Max];

// シェーダー
static CShader		g_shader;

// スタティックメッシュ（ジオメトリデータ）
static CStaticMesh g_staticmesh[PlayerBulletMod_Max];

// メッシュレンダラー
static CStaticMeshRenderer g_staticmeshrenderer[PlayerBulletMod_Max];

//ーーーーーーーーーーーーーーーーーーーーー//

//メッシュレンダラ
static CMeshRenderer g_meshrenderer[PlayerBulletMod_Max];

//マテリアル
static CMaterial g_material;

// ボックスメッシュ
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb[PlayerBulletMod_Max];

//　プレイヤープレイヤーバレット位置取得
DirectX::XMFLOAT3 GetPositionPlayerBullet(int idx) {
	return g_playerbullet[idx].position;
}

//　プレイヤーバレットマトリックス取得
DirectX::SimpleMath::Matrix GetMtxPlayerBullet(int idx) {

	Matrix bmtx;
	DX11MakeWorldMatrixRadian(
		bmtx,
		g_playerbullet[idx].scale,
		g_playerbullet[idx].rotation,
		g_playerbullet[idx].position);

	return bmtx;
}

// OBB取得
BoundingBoxOBB GetPlayerBulletOBB(int idx) {
	Matrix mtx = GetMtxPlayerBullet(idx);
	return g_obb[g_playerbullet[idx].BulletType].MakeOBB(mtx, g_playerbullet[idx].scale);
}

PlayerBullet InitializPlayerBullet() {
	PlayerBullet obj;
	obj.sts = -1;
	obj.BulletType = 0;
	obj.position = { 0,0,0 };
	obj.rotation = { 0,0,0 };
	obj.scale = { 11.0f,11.0f,20.0f };
	obj.SpeedBullet = 0.4f;
	obj.Dame = 20;

	return obj;
}

PlayerBullet InitializPlayerBullet_Sura() {

	PlayerBullet obj;
	obj.sts = -1;
	obj.BulletType = 1;
	obj.position = { 0,0,0 };
	obj.rotation = { 0,0,0 };
	obj.scale = { 10.0f,10.0f,12.0f };
	obj.SpeedBullet = 0.2f;
	obj.Dame = ENEMYBULLETDAME_Sura;

	return obj;
}

PlayerBullet InitializPlayerBullet_SuraLeaf() {

	PlayerBullet obj;
	obj.sts = -1;
	obj.BulletType = 2;
	obj.position = { 0,0,0 };
	obj.rotation = { 0,0,0 };
	obj.scale = { 9.0f,11.0f,10.0f };
	obj.SpeedBullet = 0.2f;
	obj.Dame = ENEMYBULLETDAME_SuraLeaf;

	return obj;
}

//　プレイヤーバレット初期処理
void InitPlayerBullet() {

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// 頂点シェーダ
		"shader/vertexLightingPS.hlsl");				// ピクセルシェーダ

	// モデルファイル名
	std::string filename[] = {
		u8"assets\\model\\nekobullet.obj",
		u8"assets\\model\\suraBullet.obj",
		u8"assets\\model\\leafbullet.obj",
	};
	

	for (int i = 0; i < PlayerBulletMod_Max; i++) {
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

	for (int i = 0; i < PlayerBullet_Max; i++) {
		static int cnt = 0;
		switch (cnt)
		{
		case 0://アビリティバレット初期化
			g_playerbullet[i] = InitializPlayerBullet();
			cnt++;
			break;
		case 1://Suraバレット初期化
			g_playerbullet[i] = InitializPlayerBullet_Sura();
			cnt++;
			break;
		case 2://SuraGreenバレット初期化
			g_playerbullet[i] = InitializPlayerBullet_SuraLeaf();
			cnt = 0;
			break;
		}
	}

	//ーーーーーーーーーーーーーーーーーーーー//
	
	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 0.5f);	//境界球カラー
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.TextureEnable = FALSE;

	g_material.Init(mtrl);
}

//　プレイヤーバレット更新処理
void UpdatePlayerBullet() {
	
	MovePlayerBullet();

}

//　プレイヤーバレット描画処理
void DrawPlayerBullet() {

	// デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	for (int i = 0; i < PlayerBullet_Max; i++) {
		if (g_playerbullet[i].sts == 1) {
			// ワールド変換行列生成
			Matrix mtx;
			DX11MakeWorldMatrixRadian(
				mtx,
				g_playerbullet[i].scale,							// スケール
				g_playerbullet[i].rotation,							// 姿勢
				g_playerbullet[i].position);						// 位置

			// GPUに行列をセットする
			Renderer::SetWorldMatrix(&mtx);

			// シェーダーをGPUにセット
			g_shader.SetGPU();
			// モデル描画
			g_staticmeshrenderer[g_playerbullet[i].BulletType].Draw();

			////OBB情報生成
			////境界ボックス描画
			//mtx = g_obb[g_playerbullet[i].BulletType].MakeWorldMtx(g_playerbullet[i].scale, mtx);
			//Renderer::SetWorldMatrix(&mtx);

			//// 境界ボックス描画
			//g_material.SetGPU();
			//g_meshrenderer[g_playerbullet[i].BulletType].Draw();
		}
	}

}

//　プレイヤーバレット終了処理
void ExitPlayerBullet() {

	for (int i = 0; i < PlayerBulletMod_Max; i++) {
		g_staticmeshrenderer[i].Uninit();
	}

}

//弾を撃てる条件をチェック
bool CheckFirePlayerBullet(int idx,int Type) {
	
	bool nur = false;
	//発射する弾モデルが選ばれているか
	if (g_playerbullet[idx].BulletType == Type) {
		//撃つ弾を持ってるか
		if (GetCatchBullet(Type) > 0) {
			nur = true;
		}
	}
	
	return nur;
}

//　バレット発射処理
void FirePlayerBullet(int Type) {
	
	for (int i = 0; i < PlayerBullet_Max; i++) {
		if (CheckFirePlayerBullet(i,Type)) {
			//使用済みバレット回収
			if (g_playerbullet[i].sts == 1) {

			}

			//未使用バレット取得->使用
			if (g_playerbullet[i].sts == -1) {

				//バレットがプレイヤーの座標を取得
				g_playerbullet[i].position = GetPositionPlayer();
				//プレイヤーのI番目の発射方向取得
				g_playerbullet[i].rotation = GetRotationPlayer();

				Matrix pbmtx = GetMtxPlayer();

				g_playerbullet[i].vector.x = pbmtx._31;
				g_playerbullet[i].vector.y = pbmtx._32;
				g_playerbullet[i].vector.z = pbmtx._33;

				MakeBulletFireParticle(g_playerbullet[i].position, Color(0.8f, 0.0f, 0.8f, 1.0f));

				g_playerbullet[i].sts = 1;
				PlaySound(FireSE);
				CatchCntDown(Type);
				break;//未使用が見つかればforから出る
			}
		}
	}

}

//　プレイヤーバレット移動
void MovePlayerBullet() {
	
	for (int i = 0; i < PlayerBullet_Max; i++) {
		if (g_playerbullet[i].sts == 1) {
			g_playerbullet[i].position.x -= g_playerbullet[i].vector.x * g_playerbullet[i].SpeedBullet;
			g_playerbullet[i].position.z -= g_playerbullet[i].vector.z * g_playerbullet[i].SpeedBullet;

		}
	
		if (MapOutCheck(g_playerbullet[i].position)) {
			g_playerbullet[i].sts = -1;

		}
	}

}

//　プレイヤーバレットダメージ取得
int GetPlayerBulletDame(int idx) {
	return	g_playerbullet[idx].Dame;
}

//　プレイヤーバレットとエネミーの当たり判定
bool HitCheckEnemy(int idx) {

	static	int BulletCnt = 0;
	for (int i = 0; i < PlayerBullet_Max; i++) {
		if (g_playerbullet[i].sts == 1) {
			bool sts = HitCheckOBB(GetEnemyOBB(idx), GetPlayerBulletOBB(i));
			if (sts) {
				g_playerbullet[i].sts = -1;
				BulletCnt = i;
				EnemyHPReduce(idx, g_playerbullet[i].Dame);				
				ScoreUp(1);
				SetHitStop(8);
				//エネミー死亡演出オン
				EnemyDeat(idx, i);
				return true;
			}
		}
	}

	EnemyKnockBack(BulletCnt,idx);

	return false;
}

//　プレイヤーバレットとボスの当たり判定
bool HitCheckBoss() {

	static	int knockCnt = 0;
	for (int i = 0; i < PlayerBullet_Max; i++) {
		if (g_playerbullet[i].sts == 1) {
			bool sts = HitCheckOBB(GetBossOBB(), GetPlayerBulletOBB(i));
			if (sts) {
				g_playerbullet[i].sts = -1;
				knockCnt = i;
				BossHPReduce(g_playerbullet[i].Dame);
				ScoreUp(1);
				BossDeat(i);

				return true;
			}
		}

	}

	BossKnockBack(knockCnt);

	return false;
}

