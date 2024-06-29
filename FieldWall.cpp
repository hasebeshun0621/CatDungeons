#include	"FieldWall.h"

using namespace DirectX::SimpleMath;

// 壁用
static CShader		g_shader;

//  壁メッシュ（ジオメトリデータ）
static CPlaneMesh g_planemesh;

// 壁メッシュ（レンダリングデータ）
static CMeshRenderer g_meshrenderer[FieldWall_Max];

// 壁用マテリアル
static CMaterial	g_material;

// 壁の情報(S,R,T)
struct DirectX::SimpleMath::Vector3 FieldWallPosTable[FieldWall_Max] =//位置
{
	{FieldX / 2,	-FieldX / 2,	0},
	{0,			-FieldZ / 2,	FieldZ / 2},
	{-FieldX / 2,	-FieldX / 2,	0},
	{0,			-FieldZ / 2,	-FieldZ / 2},
};

struct DirectX::SimpleMath::Vector3 FieldWallRotTable[FieldWall_Max] =// 姿勢 
{
	{ 180.0f,	 0,	90.0f},
	{ 90.0f,	0,		0},
	{ 180.0f,	0,	-90.0f},
	{-90.0f,	0,		0},
};

static DirectX::XMFLOAT3	g_scale = { 1.0f,1.0f,1.0f };			// 拡縮

// SRV
static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_srv;

//　フィールド壁初期処理
void InitFieldWall() {

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/unlitTextureVS.hlsl",				// 頂点シェーダ
		"shader/unlitTexturePS.hlsl");				// ピクセルシェーダ

	// 床メッシュ生成
	g_planemesh.Init(
		8, 8,					// 分割数
		FieldX,					// サイズ
		FieldZ,					// サイズ
		Color(1, 1, 1, 1),		// 頂点カラー
		Vector3(0, 0, -1),		// 法線ベクトル
		true);					// XZ平面

	for (int i = 0; i < FieldWall_Max; i++) {
		// メッシュレンダラ生成（床用）
		g_meshrenderer[i].Init(g_planemesh);
	}

	ID3D11Device* dev;
	ID3D11DeviceContext* devcontext;

	dev = Renderer::GetDevice();
	devcontext = Renderer::GetDeviceContext();

	bool sts = CreateSRVfromFile("assets/texture/maptile_ishizukuri_gray_02.png", dev, devcontext, &g_srv);

	if (!sts) {
		MessageBox(nullptr, "CreateSRVfromFile error(MeshWallInit)", "error", MB_OK);
	}

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

//　フィールド壁描画処理
void DrawFieldWall() {

	// 壁用シェーダセット
	g_shader.SetGPU();

	// 壁用マテリアルセット
	g_material.SetGPU();

	// テクスチャをセット
	ID3D11DeviceContext* devcontext;
	devcontext = Renderer::GetDeviceContext();
	devcontext->PSSetShaderResources(0, 1, g_srv.GetAddressOf());

	// GPUに行列をセットする
	Matrix mtx;
	for (int i = 0; i < FieldWall_Max; i++) {

		DX11MakeWorldMatrix(
			mtx,
			g_scale,							// スケール
			FieldWallRotTable[i],				// 姿勢
			FieldWallPosTable[i]);				// 位置

		Renderer::SetWorldMatrix(&mtx);
	
		// 壁描画
		g_meshrenderer[i].Draw();
	}

}


