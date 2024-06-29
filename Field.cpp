#include	"Field.h"

using namespace DirectX::SimpleMath;

// シェーダ
static CShader		g_shaderfield;

//　パネルメッシュ
static CPlaneMesh g_planemesh;

// 床メッシュ（レンダリングデータ）
static CMeshRenderer g_meshrenderer;

// 床用マテリアル
static CMaterial	g_material;

// プレイヤの情報(S,R,T)
static DirectX::XMFLOAT3	g_position = { 0.0f,0.0f,0.0f };		// 位置
static DirectX::XMFLOAT3	g_rotation = { 0.0f,0.0f,0.0f };		// 姿勢
static DirectX::XMFLOAT3	g_scale	   = { 1.0f,1.0f,1.0f };		// 拡縮

// SRV
static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_srv;

// フィールド初期処理
void InitField() {

	// 使用するシェーダーを生成
	g_shaderfield.SetShader(
		"shader/unlitTextureVS.hlsl",				// 頂点シェーダ
		"shader/unlitTexturePS.hlsl");				// ピクセルシェーダ

	// 床メッシュ生成
	g_planemesh.Init(
		10,8,					// 分割数
		FieldX,					// サイズx
		FieldZ,					// サイズz
		Color(1, 1, 1, 1),		// 頂点カラー
		Vector3(0, 0, -1),		// 法線ベクトル
		true);					// XZ平面

	// メッシュレンダラ生成（床用）
	g_meshrenderer.Init(g_planemesh);

	ID3D11Device* dev;
	ID3D11DeviceContext* devcontext;

	dev = Renderer::GetDevice();
	devcontext = Renderer::GetDeviceContext();

	bool sts = CreateSRVfromFile("assets/texture/maptile_sogen_02.png", dev, devcontext, &g_srv);
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

// フィールド描画処理
void DrawField() {

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

std::vector<VERTEX_3D> GetSquareVertices(int SquareNo) {

	std::vector<VERTEX_3D> vertices = g_planemesh.GetVertices();

	std::vector<unsigned  int> indices = g_planemesh.GetIndices();

	int idx0 = indices[SquareNo * 6+0];
	int idx1 = indices[SquareNo * 6+1];
	int idx2 = indices[SquareNo * 6+2];

	std::vector<VERTEX_3D> vlist;

	vlist.emplace_back(vertices[idx0]);
	vlist.emplace_back(vertices[idx1]);
	vlist.emplace_back(vertices[idx2]);

	idx0 = indices[SquareNo * 6 + 3];
	idx1 = indices[SquareNo * 6 + 4];
	idx2 = indices[SquareNo * 6 + 5];

	vlist.emplace_back(vertices[idx0]);
	vlist.emplace_back(vertices[idx1]);
	vlist.emplace_back(vertices[idx2]);

	return vlist;
}



//エリア場外判定 true外	false中
bool FieldOutCheck(DirectX::XMFLOAT3 pos) {
	
	bool Check = false;
	if (pos.x < -FieldX / 2) {
		Check = true;
	}
	if (pos.x > FieldX / 2) {
		Check = true;
	}	
	if (pos.z > FieldZ / 2) {
		Check = true;
	}
	if (pos.z < -FieldZ / 2) {
		Check = true;
	}

	return Check;
}