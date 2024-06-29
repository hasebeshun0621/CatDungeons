#include "TitleMenuUI_game.h"

using namespace DirectX::SimpleMath;

//　タイトルメニュー_ゲーム構造体
TitleMenuUI_game g_TitleMenuUI_g;

// シェーダー
static CShader		g_shader;

// 頂点バッファ
static ID3D11Buffer* g_VertexBuffer{};

// テクスチャリソース
static ID3D11ShaderResourceView* g_Texture{};

// マテリアル
static CMaterial	g_material{};

//　タイトルメニュー_ゲーム初期処理
void InitTitleMenuUI_game() {

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/unlitTextureVS.hlsl",				// 頂点シェーダ
		"shader/unlitTexturePS.hlsl");				// ピクセルシェーダ

	// テクスチャ読み込み
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		L"assets/TitleMenuUI_starat2.png",
		nullptr,
		&g_Texture);

	assert(g_Texture);

	// 頂点バッファ生成
	VERTEX_3D vertex[4];

	vertex[0].Position = Vector3(SCREEN_WIDTH / 3 + g_TitleMenuUI_g.X_L, g_TitleMenuUI_g.top + g_TitleMenuUI_g.Y_U, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3((SCREEN_WIDTH / 3) * 2 + g_TitleMenuUI_g.X_R, g_TitleMenuUI_g.top + g_TitleMenuUI_g.Y_U, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3(SCREEN_WIDTH / 3 + g_TitleMenuUI_g.X_L, g_TitleMenuUI_g.under + g_TitleMenuUI_g.Y_D, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3((SCREEN_WIDTH / 3) * 2 + g_TitleMenuUI_g.X_R, g_TitleMenuUI_g.under + g_TitleMenuUI_g.Y_D, 0.0f);
	vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);

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

//　タイトルメニュー_ゲーム更新処理
void UpdateTitleMenuUI_game() {
	
	if (GetSelect() == 0) {
		if (g_TitleMenuUI_g.X_L > -10) {
			g_TitleMenuUI_g.X_L--;
			g_TitleMenuUI_g.X_R++;
			g_TitleMenuUI_g.Y_U--;
			g_TitleMenuUI_g.Y_D++;
		}
	}
	else {
		g_TitleMenuUI_g.X_L = 0;
		g_TitleMenuUI_g.X_R = 0;
		g_TitleMenuUI_g.Y_U = 0;
		g_TitleMenuUI_g.Y_D = 0;
	}

}

//　タイトルメニュー_ゲーム描画処理
void DrawTitleMenuUI_game() {

	//　カメラ2D描画処理
	Camera::Draw2D();
	// シェーダーをGPUにセット
	g_shader.SetGPU();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マテリアル設定
	g_material.SetGPU();

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(g_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = Vector3(SCREEN_WIDTH / 3 + g_TitleMenuUI_g.X_L, g_TitleMenuUI_g.top + g_TitleMenuUI_g.Y_U, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3((SCREEN_WIDTH / 3) * 2 + g_TitleMenuUI_g.X_R, g_TitleMenuUI_g.top + g_TitleMenuUI_g.Y_U, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3(SCREEN_WIDTH / 3 + g_TitleMenuUI_g.X_L, g_TitleMenuUI_g.under + g_TitleMenuUI_g.Y_D, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3((SCREEN_WIDTH / 3) * 2 + g_TitleMenuUI_g.X_R, g_TitleMenuUI_g.under + g_TitleMenuUI_g.Y_D, 0.0f);
	vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

}

//　タイトルメニュー_ゲーム終了処理
void ExitTitleMenuUI_game() {

	g_VertexBuffer->Release();
	g_material.Uninit();
	g_Texture->Release();

}



