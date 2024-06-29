#include	"HitParticle.h"

using namespace DirectX;

using namespace DirectX::SimpleMath;

// マクロ定義
#define	TEXTURE_PARTICLE	"data/texture/effect000.jpg"	// 読み込むテクスチャファイル名
#define	PARTICLE_SIZE_X		(50.0f)							// ビルボードの幅
#define	PARTICLE_SIZE_Y		(50.0f)							// ビルボードの高さ
#define	VALUE_MOVE_PARTICLE	(2.0f)							// 移動速度

#define	MAX_PARTICLE		(512)							// エフェクト最大数

#define MATERIAL_DIFFUSE	XMFLOAT4(1,1,1,1)
#define MATERIAL_AMBIENT	XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR	XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE	XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER		(0.0f)

// パーティクル構造体定義
struct PARTICLE {
	XMFLOAT3 pos;			// 位置
	XMFLOAT3 rot;			// 回転
	XMFLOAT3 move;			// 移動量
	XMFLOAT4 col;			// 色
	float fSizeX;			// 幅
	float fSizeY;			// 高さ
	int nIdxShadow;			// 影ID
	int nLife;				// 寿命
	bool bUse;				// 使用しているかどうか
};

// プロトタイプ宣言
void SetVertexHitParticle(int nIdxParticle, float fSizeX, float fSizeY);
void SetColorHitParticle(int nIdxParticle, XMFLOAT4 col);

// グローバル変数
static CPlaneMesh		g_meshParticle;					// メッシュ情報
static CMaterial		g_material;						// マテリアル

static PARTICLE			g_aParticle[MAX_PARTICLE];		// パーティクルワーク
static XMFLOAT3			g_posBase;						// ビルボード発生位置

static float			g_fWidthBase = 5.0f;			// 基準の幅
static float			g_fHeightBase = 2.0f;			// 基準の高さ
static bool				g_bPause = false;				// ポーズON/OFF

// SRV（テクスチャ）
static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_srv;

// 使用するシェーダ
static CShader			g_shader;

// レンダラ
static CMeshRenderer	g_meshrenderer;

//　ヒットパーティクル初期処理
void InitHitParticle(){

	ID3D11Device* pDevice = Renderer::GetDevice();
	ID3D11DeviceContext* pDeviceContext = Renderer::GetDeviceContext();

	// 使用するシェーダーを生成
	g_shader.SetShader(
		"shader/unlitTextureVS.hlsl",				// 頂点シェーダ
		"shader/unlitTexturePS.hlsl");				// ピクセルシェーダ

	// メッシュ生成
	g_meshParticle.Init(
		1, 1,					// 分割数
		1.0f,					// サイズ
		1.0f,					// サイズ
		Color(1, 1, 1, 1),		// 頂点カラー
		Vector3(0, 0, -1),		// 法線ベクトル
		false);					// XZ平面

	// メッシュレンダラ生成（床用）
	g_meshrenderer.Init(g_meshParticle);

	ID3D11Device* dev;
	ID3D11DeviceContext* devcontext;

	dev = Renderer::GetDevice();
	devcontext = Renderer::GetDeviceContext();

	bool sts = CreateSRVfromFile("assets/Star.png", dev, devcontext, &g_srv);
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

	// パーティクル情報初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		g_aParticle[nCntParticle].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fSizeX = PARTICLE_SIZE_X;
		g_aParticle[nCntParticle].fSizeY = PARTICLE_SIZE_Y;
		g_aParticle[nCntParticle].nIdxShadow = -1;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;
	}

}

//　ヒットパーティクル更新処理
void UpdateHitParticle(){

	XMFLOAT3 rotCamera;

	// カメラの回転を取得
	rotCamera = Camera::GetRotCamera();

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		if (g_aParticle[nCntParticle].bUse) {
			// 使用中
			g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
			g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z;
			g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;


			if (g_aParticle[nCntParticle].pos.y <= g_aParticle[nCntParticle].fSizeY / 2) {
				// 着地した
				g_aParticle[nCntParticle].pos.y = g_aParticle[nCntParticle].fSizeY / 2;
				//バウンド
				g_aParticle[nCntParticle].move.y = -g_aParticle[nCntParticle].move.y * 0.9f;
			}

			g_aParticle[nCntParticle].move.x += (0.0f - g_aParticle[nCntParticle].move.x) * 0.3f;
			g_aParticle[nCntParticle].move.y -= 0.25f;
			g_aParticle[nCntParticle].move.z += (0.0f - g_aParticle[nCntParticle].move.z) * 0.3f;

			g_aParticle[nCntParticle].nLife--;
			if (g_aParticle[nCntParticle].nLife <= 0) {
				g_aParticle[nCntParticle].bUse = false;
				g_aParticle[nCntParticle].nIdxShadow = -1;
			}
			else {
				//だんだんと透明にして削除
				if (g_aParticle[nCntParticle].nLife <= 10) {
					g_aParticle[nCntParticle].col.x = 0.5f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
					g_aParticle[nCntParticle].col.y = 0.8f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
					g_aParticle[nCntParticle].col.z = 0.8f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
				}

				if (g_aParticle[nCntParticle].nLife <= 10) {
					// α値設定
					g_aParticle[nCntParticle].col.w -= 0.05f;
					if (g_aParticle[nCntParticle].col.w < 0.0f) {
						g_aParticle[nCntParticle].col.w = 0.0f;
					}
				}

				// 色の設定
				SetColorHitParticle(nCntParticle, g_aParticle[nCntParticle].col);
			}
		}
	}

}

//　ヒットパーティクル描画処理
void DrawHitParticle(){

	ID3D11DeviceContext* pDeviceContext = Renderer::GetDeviceContext();

	// 加算合成
	Renderer::SetBlendState(BS_ADDITIVE);

	// ビューマトリックスを取得
	DirectX::SimpleMath::Matrix mtxView = Camera::GetViewMtx();

	// パーティクル用シェーダセット
	g_shader.SetGPU();

	// テクスチャをセット
	ID3D11DeviceContext* devcontext;
	devcontext = Renderer::GetDeviceContext();
	devcontext->PSSetShaderResources(0, 1, g_srv.GetAddressOf());

	// 全パーティクルをビルボードとして描画する
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		if (g_aParticle[nCntParticle].bUse) {

			// ワールドマトリックスの初期化
			Matrix mtxWorld = Matrix::Identity;

			// ビルボード用行列作成
			mtxWorld._11 = mtxView._11;
			mtxWorld._12 = mtxView._21;
			mtxWorld._13 = mtxView._31;
			mtxWorld._21 = mtxView._12;
			mtxWorld._22 = mtxView._22;
			mtxWorld._23 = mtxView._32;
			mtxWorld._31 = mtxView._13;
			mtxWorld._32 = mtxView._23;
			mtxWorld._33 = mtxView._33;

			// スケール行列を作成
			Matrix mtxScale = Matrix::CreateScale(g_aParticle[nCntParticle].fSizeX, g_aParticle[nCntParticle].fSizeY, 1.0f);

			// 平行移動行列を作成
			Matrix mtxTranslate = Matrix::CreateTranslation(g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);

			// SRT生成
			Matrix mtx = mtxScale * mtxWorld * mtxTranslate;

			// GPUに行列をセットする
			Renderer::SetWorldMatrix(&mtx);

			// カラーの設定
			g_material.SetDiffuse(g_aParticle[nCntParticle].col);

			// マテリアル
			MATERIAL mtrl;
			mtrl.Ambient = Color(1, 1, 1, 1);
			mtrl.Diffuse = g_aParticle[nCntParticle].col;
			mtrl.Emission = Color(0, 0, 0, 0);
			mtrl.Specular = Color(0, 0, 0, 0);
			mtrl.Shininess = 0;
			mtrl.TextureEnable = TRUE;

			g_material.SetMaterial(mtrl);

			// 床描画
			g_meshrenderer.Draw();

			// 板ポリ描画
			//g_meshrenderer.Draw();
		}
	}

	// 半透明合成無
	Renderer::SetBlendState(1);
}

//　ヒットパーティクル終了処理
void ExitHitParticle(){

}

// 頂点座標の設定
void SetVertexHitParticle(int nIdxParticle, float fSizeX, float fSizeY){

	if (nIdxParticle >= 0 && nIdxParticle < MAX_PARTICLE) {
		g_aParticle[nIdxParticle].fSizeX = fSizeX;
		g_aParticle[nIdxParticle].fSizeY = fSizeY;
	}

}

// 頂点カラーの設定
void SetColorHitParticle(int nIdxParticle, XMFLOAT4 col){

	if (nIdxParticle >= 0 && nIdxParticle < MAX_PARTICLE) {
		g_aParticle[nIdxParticle].col = col;
	}

}

// ヒットエフェクトの設定
int SetHitParticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife){

	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		if (!g_aParticle[nCntParticle].bUse) {
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;

			// 頂点座標の設定
			SetVertexHitParticle(nCntParticle, fSizeX, fSizeY);

			// 頂点カラーの設定
			SetColorHitParticle(nCntParticle, col);

			nIdxParticle = nCntParticle;


			break;
		}
	}

	return nIdxParticle;
}

//　ヒットパーティクル
void MakeHitParticle(XMFLOAT3 pos) {

	for (int i = 0; i < 30; i++) {
		std::mt19937 rnd;
		std::uniform_real_distribution<float> dist(0.0f, XM_2PI);

		//パーティクル発生
		XMFLOAT3 move;
		float fAngle, fLength;
		int nLife;
		float fSize;

		fAngle = dist(rnd) - XM_PI;						// ラジアン

		fLength = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;


		move.x = sinf(fAngle) * fLength;

		move.y = rand() % 200 / 100.0f + g_fHeightBase;

		move.z = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;

		//エフェクト消失時間
		nLife = rand() % 5 + 20;
		//エフェクトサイズ設定
		fSize = (float)(rand() % 2 + 3);

		pos.y = fSize / 2;

		// ビルボードの設定 //カラーの初期設定
		SetHitParticle(pos, move, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife);
	}

}

//　エネミーキルパーティクル
void MakeKillParticle(XMFLOAT3 pos) {

	for (int i = 0; i < 30; i++) {
		std::mt19937 rnd;
		std::uniform_real_distribution<float> dist(0.0f, XM_2PI);

		//パーティクル発生
		XMFLOAT3 move;
		float fAngle, fLength;
		int nLife;
		float fSize;

		fAngle = dist(rnd) - XM_PI;						// ラジアン

		fLength = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;


		move.x = sinf(fAngle) * fLength;

		move.y = rand() % 200 / 100.0f + g_fHeightBase;

		move.z = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;

		//エフェクト消失時間
		nLife = rand() % 5 + 20;
		//エフェクトサイズ設定
		fSize = (float)(rand() % 2 + 3);

		pos.y = fSize / 2;

		// ビルボードの設定 //カラーの初期設定
		SetHitParticle(pos, move, XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), fSize, fSize, nLife);
	}

}

//　バレットキャッチエフェクト
void MakeBulletCatchParticle(XMFLOAT3 pos) {

	for (int i = 0; i < 10; i++) {
		std::mt19937 rnd;
		std::uniform_real_distribution<float> dist(0.0f, XM_2PI);

		//パーティクル発生
		XMFLOAT3 move;
		float fAngle, fLength;
		int nLife;
		float fSize;

		fAngle = dist(rnd) - XM_PI;						// ラジアン

		fLength = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;


		move.x = sinf(fAngle) * fLength;

		move.y = rand() % 200 / 100.0f + g_fHeightBase;

		move.z = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;

		//エフェクト消失時間
		nLife = rand() % 5 + 20;
		//エフェクトサイズ設定
		fSize = (float)(rand() % 2 + 3);

		pos.y = fSize / 2;

		// ビルボードの設定 //カラーの初期設定
		SetHitParticle(pos, move, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife);
	}

}


