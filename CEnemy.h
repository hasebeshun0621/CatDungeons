//#pragma once
//#include	<SimpleMath.h>
//#include	"CShader.h"
//#include	"CStaticMesh.h"
//#include	"CStaticMeshRenderer.h"
//#include	"Player.h"
//#include	"Field.h"
//
//class Enemy {
//	const	float RATE_ROTATE_ENEMY = 0.05f;			// 回転慣性係数
//
//	 エネミーの情報(S,R,T)
//	DirectX::SimpleMath::Vector3	m_position = { 0.0f,0.0f,0.0f };		// 位置
//	DirectX::SimpleMath::Vector3	m_rotation = { 0.0f,0.0f,0.0f };		// 姿勢
//	DirectX::SimpleMath::Vector3	m_scale = { 1.0f,1.0f,1.0f };			// 拡縮
//
//	DirectX::SimpleMath::Vector3	m_move = { 0.0f,0.0f,0.0f };			// 移動量
//
//	DirectX::SimpleMath::Vector3	m_destrot = { 0.0f,0.0f,0.0f };			// 目標回転角度
//
//	DirectX::SimpleMath::Matrix		m_mtx;
//
//	int m_fcnt = 0;//発射用カウンター
//	int m_fullcnt = 0;//弾発射間隔
//
//	 描画の為に必要な情報
//	 使用するシェーダー
//	CShader* m_shader = nullptr;
//	 スタティックメッシュ（ジオメトリデータ）
//	CStaticMesh* m_staticmesh = nullptr;
//	 メッシュレンダラー
//	CStaticMeshRenderer* m_staticmeshrenderer = nullptr;
//
//public:
//	DirectX::SimpleMath::Vector3 GetPosition() {
//		return m_position;
//	}
//
//	DirectX::SimpleMath::Vector3 GetScale() {
//		return m_scale;
//	}
//
//	DirectX::SimpleMath::Vector3 GetMove() {
//		return m_move;
//	}
//	DirectX::SimpleMath::Vector3 GetRotation() {
//		return m_rotation;;
//	}
//
//	DirectX::SimpleMath::Matrix GetMatrix() {
//		return m_mtx;
//	}
//
//	void SetShader(CShader* shader) {
//		m_shader = shader;
//	}
//
//	void SetMesh(CStaticMesh* mesh) {
//		m_staticmesh = mesh;
//	}
//
//	void SetMeshRenderer(CStaticMeshRenderer* meshrender) {
//		m_staticmeshrenderer = meshrender;
//	}
//
//	void SetPosition(DirectX::SimpleMath::Vector3 pos) {
//		m_position = pos;
//	}
//	;
//	void Update() {
//
//		 プレイヤの座標を取得
//		DirectX::SimpleMath::Vector3 playerpos = GetPositionPlayer();
//
//		 atan2を使用して角度を求める
//		m_destrot.y = atan2f(-(playerpos.x - m_position.x), -(playerpos.z - m_position.z));
//
//		 現在の向きとの差分を計算する
//		float fDiffRotY = m_destrot.y - m_rotation.y;
//
//		 補正（－１８０～１８０の範囲）
//		if (fDiffRotY > DirectX::XM_PI)
//		{
//			fDiffRotY -= DirectX::XM_PI * 2.0f;
//		}
//		if (fDiffRotY < -DirectX::XM_PI)
//		{
//			fDiffRotY += DirectX::XM_PI * 2.0f;
//		}
//
//		 回転角度計算
//		m_rotation.y += fDiffRotY * RATE_ROTATE_ENEMY;
//		if (m_rotation.y > DirectX::XM_PI)
//		{
//			m_rotation.y -= DirectX::XM_PI * 2.0f;
//		}
//		if (m_rotation.y < -DirectX::XM_PI)
//		{
//			m_rotation.y += DirectX::XM_PI * 2.0f;
//		}
//
//
//
//	}
//
//	 描画
//	void Draw() {
//
//		 デバイスコンテキスト取得
//		ID3D11DeviceContext* devicecontext;
//		devicecontext = Renderer::GetDeviceContext();
//
//		 ワールド変換行列生成
//		DirectX::SimpleMath::Matrix mtx;
//		DX11MakeWorldMatrixRadian(
//			mtx,
//			m_scale,							// スケール
//			m_rotation,							// 姿勢
//			m_position);						// 位置
//
//		m_mtx = mtx;
//
//		 GPUに行列をセットする
//		Renderer::SetWorldMatrix(&mtx);
//
//		 シェーダーをGPUにセット
//		m_shader->SetGPU();
//
//		 モデル描画
//		m_staticmeshrenderer->Draw();
//	}
//};