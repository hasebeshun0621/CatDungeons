#include	"wall.h"

using namespace DirectX::SimpleMath;

// 壁用
static CShader		g_shader;

//  壁メッシュ（ジオメトリデータ）
static CPlaneMesh g_planemesh;

// 壁メッシュ（レンダリングデータ）
static CMeshRenderer g_meshrenderer[Wall_Max];

// 壁用マテリアル
static CMaterial	g_material;

// 壁の情報(S,R,T)
struct DirectX::SimpleMath::Vector3 WallPosTable[Wall_Max]=//位置
{
	{ WallX / 2,	0,	0},
	{0,				0,	 WallZ / 2},
	{-WallX / 2,	0,	0},
	{0,				0,	-WallZ / 2},
};

struct DirectX::SimpleMath::Vector3 WallRotTable[Wall_Max]=// 姿勢 
{
	{ -90.0f,	90.0f,	0},
	{ -90.0f,	0,		0},
	{ -90.0f,	-90.0f,	0},
	{  90.0f,	0,		0},
}; 

static DirectX::XMFLOAT3	g_scale = { 1.0f,1.0f,1.0f };	// 拡縮

// SRV
static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_srv;

//　壁初期処理
void InitWall() {
	
	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/unlitTextureVS.hlsl",				// 頂点シェーダ
		"shader/unlitTexturePS.hlsl");				// ピクセルシェーダ

	// 床メッシュ生成
	g_planemesh.Init(
		10, 10,					// 分割数
		WallX,				// サイズ
		WallZ,				// サイズ
		Color(1, 1, 1, 1),		// 頂点カラー
		Vector3(0, 0, -1),		// 法線ベクトル
		true);					// XZ平面

	for (int i = 0; i < Wall_Max; i++) {
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

//　壁描画処理
void DrawWall() {

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
	for (int i = 0; i < Wall_Max; i++) {

		DX11MakeWorldMatrix(
			mtx,
			g_scale,							// スケール
			WallRotTable[i],					// 姿勢
			WallPosTable[i]);					// 位置

		Renderer::SetWorldMatrix(&mtx);
		// 壁描画
		g_meshrenderer[i].Draw();
	}

}

//　マップ外判定 true外	false中
bool MapOutCheck(DirectX::XMFLOAT3 pos) {

	bool Check = false;
	
	if (pos.x <= -WallX / 2) {
		Check = true;
	}
	if (pos.x >= WallX / 2) {
		Check = true;
	}
	if (pos.z >= WallZ / 2) {
		Check = true;
	}
	if (pos.z <= -WallZ / 2) {
		Check = true;
	}

	return Check;
}