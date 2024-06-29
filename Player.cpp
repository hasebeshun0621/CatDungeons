#include	"Player.h"

using namespace DirectX::SimpleMath;

//プレイヤー構造体
Player g_player;

// シェーダー
static CShader		g_shader;

// スタティックメッシュ（ジオメトリデータ）
static CStaticMesh g_staticmesh;

// メッシュレンダラー
static CStaticMeshRenderer g_staticmeshrenderer;

static Vector3	g_move = { 0.0f,0.0f,0.0f };			// 移動量
static Vector3	g_destrot = { 0.0f,0.0f,0.0f };			// 目標回転角度

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

// 位置取得
DirectX::XMFLOAT3 GetPositionPlayer() {
	return g_player.position;
}

//　姿勢取得
DirectX::XMFLOAT3 GetRotationPlayer() {
	return g_player.rotation;
}

//　サイズ取得
DirectX::XMFLOAT3 GetScalePlayer() {
	return g_player.scale;
}

//　プレイヤーマトリックス取得
DirectX::SimpleMath::Matrix GetMtxPlayer() {
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_player.scale,
		g_player.rotation,
		g_player.position);
	return mtx;
}

// OBB取得
BoundingBoxOBB GetPlayerOBB() {

	Matrix mtx = GetMtxPlayer();
	return g_obb.MakeOBB(mtx, g_player.scale);
}

//　移動量取得
DirectX::XMFLOAT3 GetMovePlayer() {
	return g_move;
}
//　回転目標角度取得
DirectX::XMFLOAT3 GetDestrotPlayer() {
	return g_destrot;
}
//プレイヤー当たり判定取得
bool GetPlayerHitSts() {
	return g_player.HitSts;
}
//　HPポーション所持数
int GetHPPortionCnt() {
	return g_player.HPPortionCnt;
}
//　HPポーションを増やす
void HPPortionCntUp() {
	g_player.HPPortionCnt++;
}
//　HPポーションを減らす
void HPPortionCntDown() {
	g_player.HPPortionCnt--;
}

//　今選んでいるアイテム欄
int GetItemPointer() {
	return	g_player.ItemPointer;
}
//　今選んでいるバレット
int GetSelectBullet() {
	return	g_player.ItemSideCnt;
}

//　今のプレイヤーのHPをゲット
int GetPlayerHp() {
	return g_player.HP;
}

//　プレイヤーのHPを増やす
void PlayerHPUP(int num) {

	g_player.HP += num;
	if (PlayerHP_Max <= g_player.HP) {
		g_player.HP = PlayerHP_Max;
	}

}

//　プレイヤーのHPを減らす
void PlayerHPDOWN(int num) {

	g_player.HP -= num;
	PlaySound(DameSE);

}

//　プレイヤースタミナを減らす
void PlayerSTDOWN(int num) {
	g_player.ST -= num;
}

//　今のプレイヤーのSTをゲット
int GetPlayerSt() {
	return g_player.ST;
}

//　プレイヤー初期化
Player initializePlayer() {

	Player obj;
	// プレイヤの情報(S,R,T)
	obj.position = { 0.0f,0.0f,0.0f };
	obj.rotation = { 0.0f,0.0f,0.0f };
	obj.scale = { 15.0f,15.0f,15.0f };
	obj.VALUE_MOVE_MODEL = 0.8f;						
	obj.VALUE_ROTATE_MODEL = DirectX::XM_PI * 0.1f;		
	obj.RATE_ROTATE_MODEL = 0.15f;						
	obj.RATE_MOVE_MODEL = 0.3f;							
	obj.HitSts = true;
	obj.EvasionSwitch = false;
	obj.ItemPointer = 0;
	obj.ItemSideCnt = 1;
	obj.HPPortionCnt = 0;
	obj.HP = PlayerHP_Max;
	obj.ST = 200;
	obj.STcnt = 0;

	return obj;

}

// プレイヤー初期処理
void InitPlayer() {

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// 頂点シェーダ
		"shader/vertexLightingPS.hlsl");				// ピクセルシェーダ

	// モデルファイル名
	std::string filename[] = {
		u8"assets\\model\\neko.obj"
	};

	// メッシュ生成（ジオメトリデータ）
	g_staticmesh.Init(filename[0]);

	// 描画の為のデータ生成
	g_staticmeshrenderer.Init(g_staticmesh);

//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー//
	
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

	g_player = initializePlayer();

	g_material.Init(mtrl);

}

// プレイヤー更新処理
void UpdatePlayer() {
	
	//プレイヤー落下
	if (!PlayerFall()) {
		//プレイヤー操作
		PlayerControl();
		//プレイヤー回避
		PlayerEvasion();
	}


	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LEFT)) {
		if (g_player.ItemPointer < 2) {
			g_player.ItemPointer++;
		}
		else {
			g_player.ItemPointer = 0;
		}
		PlaySound(ItemSelectSE);
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RIGHT)) {
		if (g_player.ItemPointer > 0) {
			g_player.ItemPointer--;
		}
		else {
			g_player.ItemPointer = 2;
		}
		PlaySound(ItemSelectSE);
	}
	
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP)) {
		if (g_player.ItemPointer == 2) {
			g_player.ItemSideCnt--;
			if (g_player.ItemSideCnt <= 0) {
				g_player.ItemSideCnt = PlayerBulletMod_Max-1;
			}
		}
		else {
			g_player.ItemPointer = 2;
		}
		PlaySound(BulletSE);
	}
	
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN)) {
		if (g_player.ItemPointer == 2) {
			g_player.ItemSideCnt++;
			if (g_player.ItemSideCnt >=PlayerBulletMod_Max) {
				g_player.ItemSideCnt = 1;
			}
		}
		else {
			g_player.ItemPointer = 2;
		}
		PlaySound(BulletSE);
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN)) {
		if (g_player.HitSts) {
			//自傷弾
			if (g_player.ItemPointer == ABILITY) {
				if (g_player.HP - 30 > 0) {
					FirePlayerBullet(0);
					//弾を撃つとダメージをくらう
					PlayerHPDOWN(PlayerAbilityDame);
					MakeBulletFireParticle(g_player.position, Color(1.0f, 0.0f, 0.0f, 1.0f));
				}

			}

			//回復
			if (g_player.ItemPointer == HEAL) {
				if (GetHPPortionCnt() > 0) {
					HPPortionCntDown();
					PlayerHPUP(30);
					MakeHealParticle(g_player.position);
					PlaySound(HealSE);
				}
			}

			//拾った弾
			if (g_player.ItemPointer == BULLET) {
				if (GetCatchCnt() > 0) {
					FirePlayerBullet(g_player.ItemSideCnt);

				}
			}
		}
	}
	
	if (NowBagCnt() < BagMax) {
		if (g_player.HitSts) {
			bool sts = HitCheckNet();
			if (sts) {
				ScoreUp(1);
			}
		}
	}

}

// プレイヤー描画処理
void DrawPlayer() {

	// デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ワールド変換行列生成
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_player.scale,							// スケール
		g_player.rotation,						// 姿勢
		g_player.position);						// 位置

	// GPUに行列をセットする
	Renderer::SetWorldMatrix(&mtx);

	// シェーダーをGPUにセット
	g_shader.SetGPU();												

	// モデル描画
	g_staticmeshrenderer.Draw();						
//-------------------------------------//球描画
	//// 境界ボックス描画
	//mtx = g_obb.MakeWorldMtx(g_player.scale, mtx);

	//Renderer::SetWorldMatrix(&mtx);
	//g_material.SetGPU();
	//g_meshrenderer.Draw();

}

//　プレイヤー終了処理
void ExitPlayer() {
	g_staticmeshrenderer.Uninit();
}

//　プレイヤー操作まとめ
void PlayerControl() {

	if (g_player.Control) {
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
			{// 左前移動

				float radian;
				radian = DirectX::XM_PI * 0.75f;

				g_move.x -= sinf(radian) * g_player.VALUE_MOVE_MODEL;
				g_move.z -= cosf(radian) * g_player.VALUE_MOVE_MODEL;

				// 目標角度をセット
				g_destrot.y = radian;

			}
			else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
			{// 左後移動

				float radian;
				radian = DirectX::XM_PI * 0.25f;

				g_move.x -= sinf(radian) * g_player.VALUE_MOVE_MODEL;
				g_move.z -= cosf(radian) * g_player.VALUE_MOVE_MODEL;

				// 目標角度をセット
				g_destrot.y = radian;
			}
			else
			{// 左移動

				float radian;
				radian = DirectX::XM_PI * 0.50f;

				g_move.x -= sinf(radian) * g_player.VALUE_MOVE_MODEL;
				g_move.z -= cosf(radian) * g_player.VALUE_MOVE_MODEL;

				// 目標角度をセット
				g_destrot.y = radian;
			}
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
		{
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)) {
				// 右前移動

				float radian;
				radian = -DirectX::XM_PI * 0.75f;

				g_move.x -= sinf(radian) * g_player.VALUE_MOVE_MODEL;
				g_move.z -= cosf(radian) * g_player.VALUE_MOVE_MODEL;

				// 目標角度をセット
				g_destrot.y = radian;

			}
			else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
			{// 右後移動
				float radian;
				radian = -DirectX::XM_PI * 0.25f;

				g_move.x -= sinf(radian) * g_player.VALUE_MOVE_MODEL;
				g_move.z -= cosf(radian) * g_player.VALUE_MOVE_MODEL;

				// 目標角度をセット
				g_destrot.y = radian;
			}
			else
			{// 右移動

				float radian;
				radian = -DirectX::XM_PI * 0.50f;

				g_move.x -= sinf(radian) * g_player.VALUE_MOVE_MODEL;
				g_move.z -= cosf(radian) * g_player.VALUE_MOVE_MODEL;

				// 目標角度をセット
				g_destrot.y = radian;
			}
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
		{
			// 前移動
			g_move.x -= sinf(DirectX::XM_PI) * g_player.VALUE_MOVE_MODEL;
			g_move.z -= cosf(DirectX::XM_PI) * g_player.VALUE_MOVE_MODEL;

			// 目標角度をセット
			g_destrot.y = DirectX::XM_PI * 1.0f;
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
		{
			// 後移動
			g_move.x += sinf(DirectX::XM_PI) * g_player.VALUE_MOVE_MODEL;
			g_move.z += cosf(DirectX::XM_PI) * g_player.VALUE_MOVE_MODEL;

			// 目標角度をセット
			g_destrot.y = DirectX::XM_PI * 0.0f;
		}

		PlayerDash();

		// 目標角度と現在角度との差分を求める
		float diffrot = g_destrot.y - g_player.rotation.y;
		if (diffrot > DirectX::XM_PI) {
			diffrot -= DirectX::XM_PI * 2.0f;
		}

		if (diffrot < -DirectX::XM_PI) {
			diffrot += DirectX::XM_PI * 2.0f;
		}

		// 角度速度に慣性を付ける
		g_player.rotation.y += diffrot * g_player.RATE_ROTATE_MODEL;
		if (g_player.rotation.y > DirectX::XM_PI) {
			g_player.rotation.y -= DirectX::XM_PI * 2.0f;
		}

		if (g_player.rotation.y < -DirectX::XM_PI) {
			g_player.rotation.y += DirectX::XM_PI * 2.0f;
		}

		// 位置移動
		g_player.position += g_move;

		// 移動量に慣性をかける(減速慣性)
		g_move += -g_move * g_player.RATE_MOVE_MODEL;
	}

}

//　プレイヤーダッシュ
void PlayerDash() {
	//ダッシュボタン
	//wasdのどれかを押しており
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)
		|| CDirectInput::GetInstance().CheckKeyBuffer(DIK_S) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
	{
		//スタミナが０以上でシフトを押したならダッシュ
		if ((CDirectInput::GetInstance().CheckKeyBuffer(DIK_RSHIFT) 
			|| CDirectInput::GetInstance().CheckKeyBuffer(DIK_LSHIFT)) 
				&& g_player.ST > 0)
		{

			g_player.VALUE_MOVE_MODEL = 1.2f;
			//スタミナ減少
			g_player.ST--;
			g_player.STcnt = 100;//百フレーム後にスタミナを回復
			//砂煙エフェクトを出す
			MakeDashParticle(GetPositionPlayer());
		}
		else
		{
			g_player.VALUE_MOVE_MODEL = 0.8f;
		}
	}
	//スタミナ回復
	if (g_player.ST < 200 && g_player.STcnt <= 0) {
		g_player.ST++;
	}
	else {
		g_player.STcnt--;
	}

}

//プレイヤー回避
void PlayerEvasion() {

	static float jumpfcnt = 0.0f;
	static Matrix pp;

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RCONTROL) &&
		!g_player.EvasionSwitch && g_player.ST > 30) {

		g_player.EvasionSwitch = true;
		g_player.HitSts = false;
		g_player.Control = false;
		pp = GetMtxPlayer();
		//スタミナ回復間隔
		g_player.STcnt = 100;
		PlayerSTDOWN(30);
	}

	if (g_player.EvasionSwitch) {
		//パーティクル生成
		MakeDashParticle(GetPositionPlayer());

		//プレイヤー移動
		g_player.position.z -= (pp._33 * 0.15f);
		g_player.position.x -= (pp._31 * 0.15f);

		//プレイヤー回転
		g_player.rotation.x -= 0.15f;
		//signカーブを描く
		jumpfcnt += 0.15f;
		g_player.position.y += sinf(jumpfcnt);
	}

	if (jumpfcnt >= DirectX::XM_PI * 2 || FieldOutCheck(g_player.position)) {
		g_player.EvasionSwitch = false;
		g_player.HitSts = true;
		g_player.Control = true;
		g_player.rotation.x = 0.0f;
		jumpfcnt = 0.0f;
	}

}

//　プレイヤー落下
bool PlayerFall() {

	bool sts = FieldOutCheck(g_player.position);//落下を判定する値を入れる
	static float G = 0.05f;//重力
	static float fcnt = 0.0f;//時間を表す
	float jcnt = fcnt * G;//重力加速度

	if (sts) {
		//落下速度
		g_move.y -= jcnt;
		g_player.position.y += g_move.y;
		fcnt+=0.5f;
		//落下時縦回転
		g_player.rotation.x -= 0.1f;
	}

	if (g_player.position.y <= -FallY) {
		g_player.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_player.rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_move = DirectX::SimpleMath::Vector3(0,0,0);
		//落下ダメージ
		PlayerHPDOWN(20);
		fcnt = 0.0f;
	}

	return sts;
}

//　プレイヤーポジションリセット
void ResetPosPlayer() {

	g_player.position = initializePlayer().position;
	g_player.rotation = initializePlayer().rotation;
	g_player.EvasionSwitch = false;
	g_player.HitSts = true;
	g_player.Control = true;
	Camera::Init();

}

//　プレイヤーノックバック
bool PlayerKnockBack(int idx,int type) {

	//ノックバックカント
	static int fcnt = 2;
	Matrix EB;
	int dame=0;

	switch (type)
	{
	case 1://エネミー弾
		//ヒットした弾の進行方向を取得
		EB = GetMtxEnemyBullet(idx);
		dame = GetEnemyBulletDame(idx);
		break;
	case 2://ボス弾
		EB = GetMtxBossBullet(idx);
		dame = GetBossBulletDame(idx);
		break;
	}

	if (fcnt < 10) {
		g_player.position.x -= EB._31 / fcnt * (dame / 5);
		g_player.position.z -= EB._33 / fcnt * (dame / 5);
		MakeDashParticle(g_player.position);
		fcnt++;
	}
	else {
		fcnt = 2;
		//ノックバック終わり
		return false;
	}

	//ノックバック中
	return true;
}
