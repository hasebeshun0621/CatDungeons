#include"Enemy.h"

using namespace DirectX::SimpleMath;

//　エネミー構造体
Enemy g_enemy[Enemy_Max];

// シェーダー
static CShader		g_shader;

// スタティックメッシュ（ジオメトリデータ）
static CStaticMesh g_staticmesh[EnemyMod_Max];

// メッシュレンダラー
static CStaticMeshRenderer g_staticmeshrenderer[EnemyMod_Max];

static Vector3	g_move = { 0.0f,0.0f,0.0f };			// 移動量

//ウェーブに出るエネミーの最大数
int g_WaveEnemyMax = 0;

//ーーーーーーーーーーーーーーーーーーーーー//

//メッシュレンダラ
static CMeshRenderer g_meshrenderer[EnemyMod_Max];

//マテリアル
static CMaterial g_material;

// ボックスメッシュ
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb;

//　エネミーのidx番目の位置取得
DirectX::XMFLOAT3 GetEnemyPosition(int idx) {
	return g_enemy[idx].position;
}
//　エネミーのidx番目の回転取得
DirectX::XMFLOAT3 GetEnemyRotation(int idx) {
	return g_enemy[idx].rotation;
}
//　エネミーのidx番目の回転目標を取得
DirectX::XMFLOAT3 GetEnemyDestrot(int idx) {
	return g_enemy[idx].destrot;
}
//　エネミーのidx番目のマトリックス取得
DirectX::SimpleMath::Matrix GetMtxEnemy(int idx) {

	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_enemy[idx].scale,
		g_enemy[idx].rotation,
		g_enemy[idx].position);

	return mtx;
}

//　エネミーのidx番目のOBB取得
BoundingBoxOBB GetEnemyOBB(int idx) {
	Matrix mtx = GetMtxEnemy(idx);
	return g_obb.MakeOBB(mtx, g_enemy[idx].scale);
}

//　エネミーステータスを取得
int GetStsEnemy(int idx) {
	return g_enemy[idx].sts;
}
//　エネミータイプ取得
int GetEnemyType(int idx) {
	return g_enemy[idx].EnemyType;
}
//　鍵を持っているかチェック
bool GetKeyEnemy(int idx) {
	return g_enemy[idx].KeyGet;
}
//　アイテムを持っているかチェック
bool GetItemEnemy(int idx) {
	return g_enemy[idx].ItemGet;
}

//　青スライム初期化
Enemy InitializSura() {
	
	Enemy obj;
	obj.sts = 1;
	obj.EnemyType = 0;
	obj.position.x = (rand() % (int)FieldX) - FieldX / 2;
	obj.position.y = 0;
	obj.position.z = (rand() % (int)FieldZ) - FieldZ / 2;
	obj.rotation = DirectX::XMFLOAT3{ 0,0,0 };
	obj.scale = DirectX::XMFLOAT3{ 10.0f,10.0f,10.0f };
	obj.RATE_ROTATE_ENEMY = 0.05f;
	obj.HP = ENEMYHP_Sura;
	obj.KeyGet = false;
	obj.ItemGet = false;
	obj.KnockSwitch = false;
	obj.DeathSwitch = false;
	obj.pbDeat;
	obj.Knockfcnt = 2;
	obj.fcnt = 0;
	obj.fullcnt = rand() % 50 + 100;

	return obj;
}

//　緑スライム初期化
Enemy InitializSuraGreen() {

	Enemy obj;
	obj.sts = 1;
	obj.EnemyType = 1;
	obj.position.x = (rand() % (int)FieldX) - FieldX / 2;
	obj.position.y = 0;
	obj.position.z = (rand() % (int)FieldZ) - FieldZ / 2;
	obj.rotation = DirectX::XMFLOAT3{ 0,0,0 };
	obj.scale = DirectX::XMFLOAT3{ 10.0f,10.0f,10.0f };
	obj.RATE_ROTATE_ENEMY = 0.05f;
	obj.HP = ENEMYHP_SuraGreen;
	obj.KeyGet = false;
	obj.ItemGet = false;
	obj.KnockSwitch = false;
	obj.DeathSwitch = false;
	obj.pbDeat;
	obj.Knockfcnt = 2;
	obj.fcnt = 0;
	obj.fullcnt = rand() % 50 + 100;	//弾発射間隔

	return obj;
}

//　エネミー初期処理
void InitEnemy(int idx) {
	enum Wave
	{
		Tutorial = 0,
		Wave1,
		Wave2,
		Wave3,
		Boss,
		Treasure ,
	};

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// 頂点シェーダ
		"shader/vertexLightingPS.hlsl");				// ピクセルシェーダ

	// モデルファイル名
	std::string filename[] = {
		u8"assets\\model\\sura.obj",
		u8"assets\\model\\suragreen.obj",
	};

	for (int i = 0; i < EnemyMod_Max; i++) {
		// メッシュ生成（ジオメトリデータ）
		g_staticmesh[i].Init(filename[i]);

		// 描画の為のデータ生成
		g_staticmeshrenderer[i].Init(g_staticmesh[i]);

		// OBB生成
		g_obb.Caliculate(g_staticmesh[i].GetVertices());

		// OBB描画用メッシュ生成
		Vector3 diff = g_obb.GetMax() - g_obb.GetMin();
		g_box.Init(diff.x, diff.y, diff.z, Color(1, 1, 1, 1));
		g_meshrenderer[i].Init(g_box);
	}

	//ウェーブに登場させるエネミーの数
	int sura = 0;
	int suraG = 0;
	//次に情報を入れる配列番号
	int pointa = 0;
	switch (idx)
	{
	case Tutorial:
		break;
	case Wave1:
		//ウェーブに登場させるエネミーの数を設定
		sura = 3;
		suraG = 0;
		break;
	case Wave2:
		sura = 2;
		suraG = 2;
		break;
	case Wave3:
		sura = 2;
		suraG = 3;
		break;
	case Boss:
		sura = 1;
		suraG = 1;
		break;

	}
	
	for (int i = 0; i < sura; i++) {
		g_enemy[pointa] = InitializSura();
		pointa++;
	}
	for (int i = 0; i < suraG; i++) {
		g_enemy[pointa] = InitializSuraGreen();
		pointa++;
	}

	g_WaveEnemyMax = pointa;

	//ゼロ割回避のため
	if (g_WaveEnemyMax > 0) {
		if (GetScene() != BOSS) {
			//エネミーの中の一体に鍵を持たす
			g_enemy[rand() % g_WaveEnemyMax].KeyGet = true;

			for (int i = 0; i < 2; i++) {
				//50％の確立で一帯のエネミーにアイテムを持たす
				int num = rand() % (g_WaveEnemyMax * 2);
				if (num < g_WaveEnemyMax && GetKeyEnemy(num) == false) {
					g_enemy[num].ItemGet = true;
				}
			}
		}
	}

	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 0.3f);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.TextureEnable = FALSE;

	g_material.Init(mtrl);

}

//　エネミー更新処理
void UpdateEnemy() {

	EnemyLookPlayer();
	EnemyFall();
	EnemyDeathAnime();

	for (int i = 0; i < g_WaveEnemyMax; i++) {
		if (GetStsEnemy(i) == 1&&!g_enemy[i].DeathSwitch) {
			g_enemy[i].fcnt++;
			if (g_enemy[i].fcnt >= g_enemy[i].fullcnt) {
				g_enemy[i].fcnt = 0;
				FireEnemyBullet(GetEnemyPosition(i), GetEnemyDestrot(i),g_enemy[i].EnemyType);
			}
		}
	}

	for (int i = 0; i < g_WaveEnemyMax; i++) {
		if (GetStsEnemy(i) == 1) {
			bool sts = HitCheckEnemy(i);
			if (sts) {
				if (g_enemy[i].HP <= 0) {
					KeyDrop(i);
					HPPortionDrop(i);
				}
				g_enemy[i].KnockSwitch = true;
			}
		}
	}

}

//　エネミー描画処理
void DrawEnemy() {
	
	// デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	for (int i = 0; i < g_WaveEnemyMax; i++) {
		if (g_enemy[i].sts == 1) {
			// ワールド変換行列生成
			Matrix mtx;
			DX11MakeWorldMatrixRadian(
				mtx,
				g_enemy[i].scale,							// スケール
				g_enemy[i].rotation,						// 姿勢
				g_enemy[i].position);						// 位置

			// GPUに行列をセットする
			Renderer::SetWorldMatrix(&mtx);

			// シェーダーをGPUにセット
			g_shader.SetGPU();

			// モデル描画
			g_staticmeshrenderer[g_enemy[i].EnemyType].Draw();

			//-------------------------------------//box描画
			////境界ボックス描画
			//mtx = g_obb.MakeWorldMtx(g_enemy[i].scale, mtx);
			//Renderer::SetWorldMatrix(&mtx);

			//g_material.SetGPU();
			//g_meshrenderer[g_enemy[i].EnemyType].Draw();
		}
	}

}

//　エネミー終了処理
void ExitEnemy() {

	for (int i = 0; i < EnemyBulletMod_Max; i++) {
		g_staticmeshrenderer[i].Uninit();
	}

}

//　エネミーをプレイヤーに向ける
void EnemyLookPlayer() {

	for (int i = 0; i < g_WaveEnemyMax; i++) {
		if (!g_enemy[i].DeathSwitch) {
			//プレイヤーポジションを調べる
			DirectX::SimpleMath::Vector3 playerpos = GetPositionPlayer();

			//atan2（アークタンジェント2）を使い回転目標角度を計算
			g_enemy[i].destrot.y = atan2f(-(playerpos.x - g_enemy[i].position.x), -(playerpos.z - g_enemy[i].position.z));

			//現在の向きと差分を計算する

			float fDiffRotY = g_enemy[i].destrot.y - g_enemy[i].rotation.y;

			// 補正（－１８０～１８０の範囲）
			if (fDiffRotY > DirectX::XM_PI)
			{
				fDiffRotY -= DirectX::XM_PI * 2.0f;
			}

			if (fDiffRotY < -DirectX::XM_PI)
			{
				fDiffRotY += DirectX::XM_PI * 2.0f;
			}

			// 回転角度計算
			g_enemy[i].rotation.y += fDiffRotY * g_enemy[i].RATE_ROTATE_ENEMY;
			if (g_enemy[i].rotation.y > DirectX::XM_PI)
			{
				g_enemy[i].rotation.y -= DirectX::XM_PI * 2.0f;
			}

			if (g_enemy[i].rotation.y < -DirectX::XM_PI)
			{
				g_enemy[i].rotation.y += DirectX::XM_PI * 2.0f;
			}
		}
	}

}

//　エネミータイプのＨＰを減らす
void EnemyHPReduce(int idx,int dame) {

	g_enemy[idx].HP -= dame;

}

//　エネミーノックバック
void EnemyKnockBack(int bulletcnt, int idx) {

	//ヒットした弾の進行方向を取得
	Matrix PB = GetMtxPlayerBullet(bulletcnt);
	int dame = GetPlayerBulletDame(bulletcnt);

	if (g_enemy[idx].KnockSwitch) {
		if (g_enemy[idx].Knockfcnt < 10) {
			g_enemy[idx].position.x -= PB._31 / g_enemy[idx].Knockfcnt * (float)(dame / 5);
			g_enemy[idx].position.z -= PB._33 / g_enemy[idx].Knockfcnt * (float)(dame / 5);
			//砂煙パーティクルを出す
			MakeDashParticle(g_enemy[idx].position);
			g_enemy[idx].Knockfcnt++;
		}
		else {
			g_enemy[idx].KnockSwitch = false;
			g_enemy[idx].Knockfcnt = 2;
		}
	}

}

//　エネミー落下
void EnemyFall() {

	static float G = 0.05f;//重力
	static float fcnt = 0;//時間を表す

	for (int i = 0; i < Enemy_Max; i++) {
		if (g_enemy[i].sts == 1) {
			//落下を判定する
			bool sts = FieldOutCheck(g_enemy[i].position);

			if (sts) {
				float jcnt = fcnt * G;//重力加速度
				//落下速度
				g_enemy[i].position.y -= jcnt;
				fcnt++;
			}

			if (g_enemy[i].position.y <= -FallY) {
				g_enemy[i].sts = -1;
				FallKeyDrop(i);
				FallHPPotionDrop(i);
				fcnt = 0;
			}
		}
	}

}

//　エネミー死亡チェック
void EnemyDeat(int idx,int bidx) {

	if (g_enemy[idx].HP <= 0) {
		g_enemy[idx].DeathSwitch = true;
		//ヒットした弾の進行方向を取得
		g_enemy[idx].pbDeat = GetMtxPlayerBullet(bidx);
		MakeKillParticle(GetPositionPlayerBullet(bidx));
		PlaySound(KOSE);
	}
	else {
		MakeHitParticle(GetPositionPlayerBullet(bidx));
		PlaySound(HitSE);
	}

}

//　エネミー死亡演出
void EnemyDeathAnime() {

	for (int i = 0; i < Enemy_Max; i++) {
		if (g_enemy[i].DeathSwitch) {
			Matrix pb = g_enemy[i].pbDeat;
			g_enemy[i].position.x -= pb._31/3;
			g_enemy[i].position.y += 1.5f;
			g_enemy[i].position.z -= pb._33/3;
			g_enemy[i].rotation.x+=0.4f;
		}
	}

}


