#pragma once

#include	<SimpleMath.h>

class Camera {
private:
	static DirectX::SimpleMath::Vector3	m_Position;
	static DirectX::SimpleMath::Vector3	m_Rotation;
	static DirectX::SimpleMath::Vector3	m_Scale;

	static DirectX::SimpleMath::Vector3	m_Target;
	static DirectX::SimpleMath::Matrix	m_ViewMatrix;

	static float m_TargetLength;

	static DirectX::SimpleMath::Vector3 m_TargetDest;
	static DirectX::SimpleMath::Vector3 m_PositionDest;

	static float m_HeightCameraP;
	static float m_HeightCameraR;

	static int m_AngleType;

public:
	//カメラ初期処理
	static void Init();
	//カメラ更新処理
	static void Update();
	//カメラ描画処理
	static void Draw();
	//カメラ2D描画処理
	static void Draw2D();
	//カメラ終了処理
	static void Uninit();

	// カメラの回転を取得
	static DirectX::SimpleMath::Vector3 GetRotCamera() {
		return m_Rotation;
	}

	// ビューマトリックスを取得
	static DirectX::SimpleMath::Matrix GetViewMtx() {
		return m_ViewMatrix;
	}
};