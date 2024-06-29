#include	"TitleEnemy.h"

using namespace DirectX::SimpleMath;

//　タイトル用エネミー構造体
TitleEnemy g_enemy;

// シェーダー
static CShader		g_shader;

// スタティックメッシュ（ジオメトリデータ）
static CStaticMesh g_staticmesh[EnemyMod_Max];

// メッシュレンダラー
static CStaticMeshRenderer g_staticmeshrenderer[EnemyMod_Max];

//　タイトル用エネミー初期処理
void InitTitleEnemy() {

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// 頂点シェーダ
		"shader/vertexLightingPS.hlsl");				// ピクセルシェーダ

	// モデルファイル名
	std::string filename[] = {
		u8"assets\\model\\sura.obj",
		u8"assets\\model\\suragreen.obj",
	};

	g_enemy.position = { -270.0f,0.0f,-90.0f };		// 位置
	g_enemy.rotation = { 0.0f,0.0f,0.0f };			// 姿勢
	g_enemy.scale = { 15.0f,15.0f,15.0f };			// 拡縮

	for (int i = 0; i < EnemyMod_Max; i++) {

		// メッシュ生成（ジオメトリデータ）
		g_staticmesh[i].Init(filename[i]);

		// 描画の為のデータ生成
		g_staticmeshrenderer[i].Init(g_staticmesh[i]);
	}

	//プレイヤーモデルを右に向かす
	g_enemy.rotation.y = -DirectX::XM_PI * 0.50f;

}

//　タイトル用エネミー更新処理
void UpdateTitleEnemy() {

	if (GetScene() == TITLE) {
		g_enemy.position.x++;	
		if (g_enemy.position.x >= 200) {
			g_enemy.position.x = -200;
			g_enemy.enetype++;
		}
	}

	if (GetScene() == TITLEMENU) {
		g_enemy.position.x--;
		g_enemy.rotation.y = DirectX::XM_PI * 0.50f;
		if (g_enemy.position.x <= -200) {
			g_enemy.position.x = 200;
			g_enemy.enetype++;
		}
	}

	if (g_enemy.enetype == EnemyMod_Max) {
		g_enemy.enetype = 0;
	}
}

//　タイトル用エネミー描画処理
void DrawTitleEnemy() {

	//カメラ描画処理
	Camera::Draw();
	
	// デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ワールド変換行列生成
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_enemy.scale,							// スケール
		g_enemy.rotation,							// 姿勢
		g_enemy.position);						// 位置

	// GPUに行列をセットする
	Renderer::SetWorldMatrix(&mtx);

	// シェーダーをGPUにセット
	g_shader.SetGPU();

	// モデル描画
	g_staticmeshrenderer[g_enemy.enetype].Draw();

}

//　タイトル用エネミー終了処理
void ExitTitleEnemy() {

	for (int i = 0; i < EnemyMod_Max; i++) {
		g_staticmeshrenderer[i].Uninit();
	}

}

