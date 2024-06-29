#include "renderer.h"
#include "camera.h"
#include "CDirectInput.h"
#include "game.h"
#include "Player.h"

using namespace DirectX::SimpleMath;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
constexpr float VIEW_ANGLE = DirectX::XM_PI / 4.0f;							// 視野角
constexpr float VIEW_ASPECT = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// ビュー平面のアスペクト比	
constexpr float VIEW_NEAR_Z = 1.0f;											// ビュー平面のNearZ値
constexpr float VIEW_FAR_Z = 1000.0f;										// ビュー平面のFarZ値
constexpr float VALUE_MOVE_CAMERA = 2.0f;									// カメラの移動量
constexpr float VALUE_ROTATE_CAMERA = DirectX::XM_PI * 0.01f;				// カメラの回転量

constexpr float CAM_POS_P_X = 0.0f;			// カメラの視点初期位置(X座標)
constexpr float CAM_POS_P_Y = 100.0f;		// カメラの視点初期位置(Y座標)
constexpr float CAM_POS_P_Z = -200.0f;		// カメラの視点初期位置(Z座標)
constexpr float CAM_POS_LOOKAT_X = 0.0f;	// カメラの注視点初期位置(X座標)
constexpr float CAM_POS_LOOKAT_Y = 0.0f;	// カメラの注視点初期位置(Y座標)
constexpr float CAM_POS_LOOKAT_Z = 0.0f;	// カメラの注視点初期位置(Z座標)

constexpr float	INTERVAL_CAMERA_R = 12.5f;		// モデルの視線の先までの距離
constexpr float	RATE_CHASE_CAMERA_P = 0.30f;	// カメラの視点への補正係数
constexpr float	RATE_CHASE_CAMERA_R = 0.20f;	// カメラの注視点への補正係数
constexpr float	INTERVAL_CAMERA_R_MOVE = 20.5f;	// モデルの視線の先までの距離

constexpr float	CHASE_HEIGHT_P = 200.0f;		// 追跡時の視点の高さ
constexpr float	CHASE_HEIGHT_R = 10.0f;			// 追跡時の注視点の高さ

// static 変数
DirectX::SimpleMath::Vector3	Camera::m_Position = DirectX::SimpleMath::Vector3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);//カメラポシェット
DirectX::SimpleMath::Vector3	Camera::m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);					 //カメラ回転
DirectX::SimpleMath::Vector3	Camera::m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);;						 //カメラサイズ

DirectX::SimpleMath::Vector3	Camera::m_Target = DirectX::SimpleMath::Vector3(CAM_POS_LOOKAT_X, CAM_POS_LOOKAT_Y, CAM_POS_LOOKAT_Z);
DirectX::SimpleMath::Matrix		Camera::m_ViewMatrix{};
DirectX::SimpleMath::Vector3	Camera::m_TargetDest= DirectX::SimpleMath::Vector3(CAM_POS_LOOKAT_X, CAM_POS_LOOKAT_Y, CAM_POS_LOOKAT_Z);
DirectX::SimpleMath::Vector3	Camera::m_PositionDest = DirectX::SimpleMath::Vector3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);

float Camera::m_TargetLength = 0.0f;

float Camera::m_HeightCameraP;
float Camera::m_HeightCameraR;

int Camera::m_AngleType = 2;

void Camera::Init()
{
	m_Position = DirectX::XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);
	m_Target = DirectX::XMFLOAT3(CAM_POS_LOOKAT_X, CAM_POS_LOOKAT_Y, CAM_POS_LOOKAT_Z);
	m_Rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	// カメラと注視点の距離を求める
	m_TargetLength = (m_Position - m_Target).Length();

	// 追尾する時の位置（高さ）
	m_HeightCameraP = CHASE_HEIGHT_P;

	// 追尾する時の注視点の位置（高さ）
	m_HeightCameraR = CHASE_HEIGHT_R;
}

void Camera::Uninit()
{

}

void Camera::Update()
{
	DirectX::XMFLOAT3	posModel;				// モデル座標
	DirectX::XMFLOAT3	rotModel;				// モデル姿勢
	DirectX::XMFLOAT3	moveModel;				// モデル移動量

	enum angle
	{
		FPS,
		TPS,
		Chase
	};

	float		fIntervalCamera;				// カメラとモデルの距離
	float		fLengthMove;					// 移動量
	float		fRateChaseCameraPos;			// カメラ位置の移動率
	float		fRateChaseCameraLookat;			// 注視点の移動率

	posModel = GetPositionPlayer();				// プレイヤの位置を取得
	rotModel = GetRotationPlayer();				// プレイヤの向き取得
	moveModel = GetMovePlayer();				// プレイヤの移動量取得

	DirectX::XMFLOAT3 scale(1, 1, 1);
	DirectX::XMFLOAT3 trans(0, 0, 0);

	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		scale,
		rotModel,
		trans);

	//z軸取り出し
	Vector3 zAxis;
	zAxis.x = mtx._31;
	zAxis.y = mtx._32;
	zAxis.z = mtx._33;

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_I)) {
		m_AngleType = FPS;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_K)) {
		m_AngleType = TPS;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_M)) {
		m_AngleType = Chase;
	}

	// モデルが移動中か？
	if (moveModel.x < -0.05f || moveModel.x > 0.05f
		|| moveModel.z < -0.05f || moveModel.z > 0.05f)
	{
		fIntervalCamera = INTERVAL_CAMERA_R + sqrtf(moveModel.x * moveModel.x + moveModel.z * moveModel.z) * 10.0f;
		fRateChaseCameraLookat = 0.10f;
		fRateChaseCameraPos = 0.30f;
	}
	else
	{
		// モデルが停止中
		fIntervalCamera = INTERVAL_CAMERA_R + sqrtf(moveModel.x * moveModel.x + moveModel.z * moveModel.z) * 6.0f;
		fRateChaseCameraLookat = 0.1f;
		fRateChaseCameraPos = 0.30f;
	}
	fLengthMove = sqrtf(moveModel.x * moveModel.x + moveModel.z * moveModel.z) * 6.0f;

	switch (m_AngleType)
	{
	case FPS:
		//追加FPS
		m_Target = posModel - zAxis * 50.0f;
		m_Position = posModel - zAxis * 8.0f;
		m_Position.y = 15.0f;
		break;
	case TPS:
		m_Target = posModel + zAxis * -35.0f;
		m_Position = posModel + zAxis * 40.0f;
		m_Position.y = 45.0f;

		break;
	case Chase:

		//// カメラの旋回
		//if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_Z))
		//{
		//	// 視点旋回「左」
		//	m_Rotation.y += VALUE_ROTATE_CAMERA;
		//	if (m_Rotation.y > DirectX::XM_PI)
		//	{
		//		m_Rotation.y -= DirectX::XM_PI * 2.0f;
		//	}
		//	m_Position.x = m_Target.x - sinf(m_Rotation.y) * m_TargetLength;
		//	m_Position.z = m_Target.z - cosf(m_Rotation.y) * m_TargetLength;
		//}
		//if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_C))
		//{
		//	// 視点旋回「右」
		//	m_Rotation.y -= VALUE_ROTATE_CAMERA;
		//	if (m_Rotation.y < -DirectX::XM_PI)
		//	{
		//		m_Rotation.y += DirectX::XM_PI * 2.0f;
		//	}
		//	m_Position.x = m_Target.x - sinf(m_Rotation.y) * m_TargetLength;
		//	m_Position.z = m_Target.z - cosf(m_Rotation.y) * m_TargetLength;
		//}

		// 注視点の目標位置
		m_TargetDest.x = posModel.x -sin(rotModel.y) * fIntervalCamera;
		m_TargetDest.y = posModel.y - m_HeightCameraR;
		m_TargetDest.z = posModel.z -cos(rotModel.y) * fIntervalCamera;

		// 視点の目標位置
		m_PositionDest.x = posModel.x -sinf(m_Rotation.y) * m_TargetLength - sin(rotModel.y) * fLengthMove;
		m_PositionDest.y = posModel.y + m_HeightCameraP;
		m_PositionDest.z = posModel.z - cosf(m_Rotation.y) * m_TargetLength - cos(rotModel.y) * fLengthMove;

		// 注視点の補正
		m_Target.x += (m_TargetDest.x - m_Target.x) * fRateChaseCameraLookat;
		m_Target.y += (m_TargetDest.y - m_Target.y) * 0.10f;
		m_Target.z += (m_TargetDest.z - m_Target.z) * fRateChaseCameraLookat;
		
		// 視点の補正
		m_Position += (m_PositionDest - m_Position) * fRateChaseCameraPos;
		
		break;

	}
	// ライト
	LIGHT light{};
	light.Enable = true;
	light.Direction = Vector4(m_ViewMatrix._13, m_ViewMatrix._23, m_ViewMatrix._33, 0.0f);//ライトの照らす方向
	light.Direction.Normalize();
	light.Ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
	light.Diffuse = Color(1.5f, 1.5f, 1.5f, 1.0f);
	Renderer::SetLight(light);

}

void Camera::Draw()
{
	// ビュー変換後列作成
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, m_Target, up);		// 左手系にした　20230511 by suzuki.tomoki

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクション行列の生成
	float fieldOfView = VIEW_ANGLE;    // 視野角
	
	float aspectRatio = VIEW_ASPECT;	// アスペクト比	
	float nearPlane = VIEW_NEAR_Z;       // ニアクリップ
	float farPlane = VIEW_FAR_Z;      // ファークリップ

	//プロジェクション行列の生成
	Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki


	Renderer::SetProjectionMatrix(&projectionMatrix);
}

void Camera::Draw2D() {
	Matrix projection;
	projection = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,
		static_cast<float>(SCREEN_WIDTH),//画面の最小Ｘ
		static_cast<float>(SCREEN_HEIGHT),//画面の最小Ｙ
		0.0f,//画面の最大Ｙ
		0.0f,
		1.0f);

	Renderer::SetProjectionMatrix(&projection);

	//ワールド変換行列セット
	Matrix world;
	world = Matrix::Identity;//単位行列にする
	Renderer::SetWorldMatrix(&world);

	//ビュー変換行列セット
	Matrix view;
	view = Matrix::Identity;

	Renderer::SetViewMatrix(&view);

}