#include	"Poison.h"

using namespace DirectX::SimpleMath;

// 床用
static CShader		g_shaderfield;

//  床メッシュ（ジオメトリデータ）
static CPlaneMesh g_planemesh;

// 床メッシュ（レンダリングデータ）
static CMeshRenderer g_meshrenderer;

// 床用マテリアル
static CMaterial	g_material;

// プレイヤの情報(S,R,T)
static DirectX::XMFLOAT3	g_position = { 0.0f,-70.0f,0.0f };		// 位置
static DirectX::XMFLOAT3	g_rotation = { 0.0f,0.0f,0.0f };		// 姿勢
static DirectX::XMFLOAT3	g_scale = { 1.0f,1.0f,1.0f };			// 拡縮

// SRV
static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_srv;

//　毒沼初期処理
void InitPoison() {
	
	// 使用するシェーダーを生成
	g_shaderfield.SetShader(
		"shader/unlitTextureVS.hlsl",				// 頂点シェーダ
		"shader/unlitTexturePS.hlsl");				// ピクセルシェーダ

	// 床メッシュ生成
	g_planemesh.Init(
		15, 15,					// 分割数
		1000,					// サイズx
		1000,					// サイズz
		Color(1, 1, 1, 1),		// 頂点カラー
		Vector3(0, 0, -1),		// 法線ベクトル
		true);					// XZ平面

	// メッシュレンダラ生成（床用）
	g_meshrenderer.Init(g_planemesh);

	ID3D11Device* dev;
	ID3D11DeviceContext* devcontext;

	dev = Renderer::GetDevice();
	devcontext = Renderer::GetDeviceContext();

	bool sts = CreateSRVfromFile("assets/texture/maptile_dokunuma.png", dev, devcontext, &g_srv);
	if (!sts) {
		MessageBox(nullptr, "CreateSRVfromFile error(MeshWallInit)", "error", MB_OK);
	}

	g_position = { 0.0f,-70.0f,0.0f };		// 位置
	g_rotation = { 0.0f,0.0f,0.0f };		// 姿勢
	g_scale = { 1.0f,1.0f,1.0f };			// 拡縮

	// マテリアル初期化
	MATERIAL mtrl;
	mtrl.Ambient = Color(1, 1, 1, 1);
	mtrl.Diffuse = Color(1, 1, 1, 1);
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.TextureEnable = TRUE;
	g_material.Init(mtrl);

}

//　毒沼更新処理
void UpdatePoison(){

	static int fcnt = 0;
	static bool nur = false;

	if (fcnt < 100 && nur==false) {
		g_position.y += 0.05f;
		fcnt++;
	}
	else {
		nur = true;
	}

	if (fcnt > 0 && nur) {
		g_position.y -= 0.051f;
		fcnt--;
	}
	else {
		nur = false;
	}

}

//　毒沼描画処理
void DrawPoison() {

	// 床用シェーダセット
	g_shaderfield.SetGPU();

	// 床用マテリアルセット
	g_material.SetGPU();

	// テクスチャをセット
	ID3D11DeviceContext* devcontext;
	devcontext = Renderer::GetDeviceContext();
	devcontext->PSSetShaderResources(0, 1, g_srv.GetAddressOf());

	// GPUに行列をセットする
	Matrix mtx;
	DX11MakeWorldMatrix(
		mtx,
		g_scale,							// スケール
		g_rotation,							// 姿勢
		g_position);						// 位置

	Renderer::SetWorldMatrix(&mtx);

	// 床描画
	g_meshrenderer.Draw();

}
