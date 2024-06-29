#include	"TitlePlayer.h"

using namespace DirectX::SimpleMath;

// シェーダー
static CShader		g_shader;

// スタティックメッシュ（ジオメトリデータ）
static CStaticMesh g_staticmesh;

// メッシュレンダラー
static CStaticMeshRenderer g_staticmeshrenderer;

// RTS情報
Vector3 g_position = { -200.0f,0.0f,0.0f };		// 位置
Vector3 g_rotation = { 0.0f,0.0f,0.0f };		// 姿勢
Vector3 g_scale = { 15.0f,15.0f,15.0f };		// 拡縮

//　タイトル用プレイヤー初期処理
void InitTitlePlayer() {

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// 頂点シェーダ
		"shader/vertexLightingPS.hlsl");				// ピクセルシェーダ

	// モデルファイル名
	std::string filename[] = {
		u8"assets\\model\\neko.obj"
	};

	g_position = { -200.0f,0.0f,-90.0f };		// 位置
	g_rotation = { 0.0f,0.0f,0.0f };			// 姿勢
	g_scale = { 15.0f,15.0f,15.0f };			// 拡縮

	// メッシュ生成（ジオメトリデータ）
	g_staticmesh.Init(filename[0]);

	// 描画の為のデータ生成
	g_staticmeshrenderer.Init(g_staticmesh);

	//プレイヤーモデルを右に向かす
	g_rotation.y = -DirectX::XM_PI * 0.50f;

}

//　タイトル用プレイヤー更新処理
void UpdateTitlePlayer() {
	
	if (GetScene() == TITLE) {
		g_position.x++;
		if (g_position.x >= 200) {
			g_position.x = -200;
		}
	}

	if (GetScene() == TITLEMENU) {
		g_position.x--;
		g_rotation.y = DirectX::XM_PI * 0.50f;
		if (g_position.x <= -200) {
			g_position.x = 200;
		}
	}
}

//　タイトル用プレイヤー描画処理
void DrawTitlePlayer() {

	//カメラ描画処理
	Camera::Draw();

	// デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ワールド変換行列生成
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_scale,							// スケール
		g_rotation,							// 姿勢
		g_position);						// 位置

	// GPUに行列をセットする
	Renderer::SetWorldMatrix(&mtx);

	// シェーダーをGPUにセット
	g_shader.SetGPU();

	// モデル描画
	g_staticmeshrenderer.Draw();

}

//　タイトル用プレイヤー終了処理
void ExitTitlePlayer() {

	g_staticmeshrenderer.Uninit();

}

