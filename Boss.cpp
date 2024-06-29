#include"Boss.h"

using namespace DirectX::SimpleMath;

//　ボス構造体
Boss g_boss;

// シェーダー
static CShader		g_shader;

// スタティックメッシュ（ジオメトリデータ）
static CStaticMesh g_staticmesh;

// メッシュレンダラー
static CStaticMeshRenderer g_staticmeshrenderer;

static Vector3	g_move = { 0.0f,0.0f,0.0f };			// 移動量

//メッシュレンダラ
static CMeshRenderer g_meshrenderer;

//マテリアル
static CMaterial g_material;

// ボックスメッシュ
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb;

//　ボスポジション取得
DirectX::XMFLOAT3 GetBossPosition() {
	return g_boss.position;
}
//　ボス回転取得
DirectX::XMFLOAT3 GetBossRotation() {
	return g_boss.rotation;
}
//　ボス回転目標角度取得
DirectX::XMFLOAT3 GetBossDestrot() {
	return g_boss.destrot;
}

//　ボスマトリックス取得
DirectX::SimpleMath::Matrix GetMtxBoss() {

	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_boss.scale,
		g_boss.rotation,
		g_boss.position);

	return mtx;
}

// ボスOBB取得
BoundingBoxOBB GetBossOBB() {
	Matrix mtx = GetMtxBoss();
	return g_obb.MakeOBB(mtx, g_boss.scale);
}

//　ボスステータス取得
int GetStsBoss() {
	return g_boss.sts;
}

//　ボス初期化
Boss InitializBoss() {

	Boss obj;
	obj.sts = 1;
	obj.Type = 0;
	obj.position.x = (rand() % (int)FieldX) - FieldX / 2;
	obj.position.y = 0;
	obj.position.z = (rand() % (int)FieldZ) - FieldZ / 2;
	obj.rotation = DirectX::XMFLOAT3{ 0,0,0 };
	obj.scale = DirectX::XMFLOAT3{ 5.0f,5.0f,5.0f };
	obj.RATE_ROTATE_ENEMY = 0.05f;
	obj.MoveSpeed = 0.01f;
	obj.HP = ENEMYHP_BOSS;
	obj.KeyGet = false;
	obj.ItemGet = false;
	obj.KnockSwitch = false;
	obj.Knockfcnt = 2;
	obj.DeathSwitch = false;
	obj.pbDeat;
	obj.fcnt = 0;
	obj.fullcnt = rand() % 10 + 40;

	return obj;
}

//　ボス初期処理
void InitBoss() {

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// 頂点シェーダ
		"shader/vertexLightingPS.hlsl");				// ピクセルシェーダ

	// モデルファイル名
	std::string filename[] = {
		u8"assets\\model\\boss.obj",
	};

	// メッシュ生成（ジオメトリデータ）
	g_staticmesh.Init(filename[0]);

	// 描画の為のデータ生成
	g_staticmeshrenderer.Init(g_staticmesh);

	// OBB生成
	g_obb.Caliculate(g_staticmesh.GetVertices());

	// OBB描画用メッシュ生成
	Vector3 diff = g_obb.GetMax() - g_obb.GetMin();
	g_box.Init(diff.x, diff.y, diff.z, Color(1, 1, 1, 1));
	g_meshrenderer.Init(g_box);

	g_boss = InitializBoss();

	//エネミーの中の一体に鍵を持たす
	g_boss.KeyGet = true;

	//50％の確立で一帯のエネミーにアイテムを持たす
	g_boss.ItemGet = true;

	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 0.3f);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.TextureEnable = FALSE;

	g_material.Init(mtrl);

}

//　ボス更新処理
void UpdateBoss() {

	BossLookPlayer();
	BossFall();
	BossDeathAnime();
	BossMove();

	if (GetStsBoss() == 1) {
		g_boss.fcnt++;
		if (g_boss.fcnt >= g_boss.fullcnt) {
			g_boss.fcnt = 0;
			FireBossBullet(GetBossPosition(), GetBossDestrot());
		}

		bool sts = HitCheckBoss();
		if (sts) {
			if (g_boss.HP <= 0) {
				SetHitStop(25);
			}
			g_boss.KnockSwitch = true;
		}
	}

}

//　ボス描画処理
void DrawBoss() {

	// デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();
	
	if (g_boss.sts == 1) {
			// ワールド変換行列生成
			Matrix mtx;
			DX11MakeWorldMatrixRadian(
				mtx,
				g_boss.scale,							// スケール
				g_boss.rotation,						// 姿勢
				g_boss.position);						// 位置

			// GPUに行列をセットする
			Renderer::SetWorldMatrix(&mtx);

			// シェーダーをGPUにセット
			g_shader.SetGPU();

			// モデル描画
			g_staticmeshrenderer.Draw();
			//-------------------------------------//box描画
			////境界ボックス描画
			//mtx = g_obb.MakeWorldMtx(g_boss.scale, mtx);
			//Renderer::SetWorldMatrix(&mtx);
			//g_material.SetGPU();
			//g_meshrenderer.Draw();
	}

}

//　ボス終了処理
void ExitBoss() {
		g_staticmeshrenderer.Uninit();
}

//　ボスをプレイヤーに向ける
void BossLookPlayer() {

	//プレイヤーポジションを調べる
	DirectX::SimpleMath::Vector3 playerpos = GetPositionPlayer();

	//atan2（アークタンジェント2）を使い回転目標角度を計算
	g_boss.destrot.y = atan2f(-(playerpos.x - g_boss.position.x), -(playerpos.z - g_boss.position.z));

	//現在の向きと差分を計算する
	float fDiffRotY = g_boss.destrot.y - g_boss.rotation.y;

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
	g_boss.rotation.y += fDiffRotY * g_boss.RATE_ROTATE_ENEMY;
	if (g_boss.rotation.y > DirectX::XM_PI) {
		g_boss.rotation.y -= DirectX::XM_PI * 2.0f;
	}

	if (g_boss.rotation.y < -DirectX::XM_PI) {
		g_boss.rotation.y += DirectX::XM_PI * 2.0f;
	}

}

//　ボスのHPを減らす
void BossHPReduce(int dame) {
	g_boss.HP -= dame;
}

//ボスノックバック
void BossKnockBack(int idx) {
	
	//ヒットした弾の進行方向を取得
	Matrix PB = GetMtxPlayerBullet(idx);
	int dame = GetPlayerBulletDame(idx);

	if (g_boss.KnockSwitch) {
		if (g_boss.Knockfcnt < 10) {
			g_boss.position.x -= PB._31 * (dame / 10) / g_boss.Knockfcnt;
			g_boss.position.z -= PB._33 * (dame / 10) / g_boss.Knockfcnt;
			g_boss.Knockfcnt++;
		}
		else {
			g_boss.KnockSwitch = false;
			g_boss.Knockfcnt = 2;

		}
	}

}

//ボス落下
void BossFall() {
	
	static float G = 0.05f;//重力
	static float fcnt = 0;//時間を表す

	if (g_boss.sts == 1) {
		//落下を判定する関数を入れる
		bool sts = FieldOutCheck(g_boss.position);

		if (sts) {
			float jcnt = fcnt * G;//重力加速度
			//落下速度
			g_boss.position.y -= jcnt;
			fcnt++;
		}
	
		//ボスの消える高さ
		if (g_boss.position.y <= -200.0f) {
			g_boss.sts = -1;
			fcnt = 0;
		}
	}

}

//　ボス移動
void BossMove() {

	static	float radian;
	static	int direction;
	static float movefcnt;
	static bool null=true;
	//エネミーの場外をチェック
	static DirectX::XMFLOAT3 outcheck = {0.0f,0.0f,0.0f};

	if (null) {
		srand(time(NULL));//ランダムシード変更
		//ボスが動く方向8方向
		direction = rand() % 8;
		//ボスの移動量
		movefcnt = rand() % 10 + 40.0f;
		null = false;
	}

	if (movefcnt > 0&&!null) {
		movefcnt--;
		switch (direction)
		{
		case 0://後
			radian = DirectX::XM_PI * 0.0f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;
			break;
		case 1://前
			radian = DirectX::XM_PI * 1.0f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;
			break;
		case 2://右
			radian = DirectX::XM_PI * 0.5f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;
			break;
		case 3://左
			radian = -DirectX::XM_PI * 0.5f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;
			break;
		case 4://右後ろ
			radian = DirectX::XM_PI * 0.25f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;
			break;
		case 5://右前
			radian = DirectX::XM_PI * 0.75f;
			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;

			break;
		case 6://左後
			radian = -DirectX::XM_PI * 0.25f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;
			break;
		case 7://左前
			radian = -DirectX::XM_PI * 0.75f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;

			break;
		}
		outcheck = g_boss.position+g_move;
		//場外をチェック
		if (FieldOutCheck(outcheck)) {
			null = true;
			g_move = DirectX::XMFLOAT3{0.0f,0.0f,0.0f};
		}
		else {
			g_boss.position.x += g_move.x;
			g_boss.position.z += g_move.z;
		}
	}
	else {
		null = true;
	}

}

//　ボス死亡チェック
void BossDeat(int bidx) {

	if (g_boss.HP <= 0) {
		g_boss.DeathSwitch = true;
		//ヒットした弾の進行方向を取得
		g_boss.pbDeat = GetMtxPlayerBullet(bidx);
		MakeKillParticle(GetPositionPlayerBullet(bidx));
		PlaySound(KOSE);
	}
	else {
		MakeHitParticle(GetPositionPlayerBullet(bidx));
		PlaySound(HitSE);
	}

}

//　ボス死亡演出
void BossDeathAnime() {

	if (g_boss.DeathSwitch) {
		Matrix pb = g_boss.pbDeat;
		g_boss.position.x -= pb._31 / 3;
		g_boss.position.y += 2.0f;
		g_boss.position.z -= pb._33 / 3;
		g_boss.rotation.y += 0.4f;
	}

}



