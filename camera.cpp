#include "renderer.h"
#include "camera.h"
#include "CDirectInput.h"
#include "game.h"
#include "Player.h"

using namespace DirectX::SimpleMath;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
constexpr float VIEW_ANGLE = DirectX::XM_PI / 4.0f;							// ����p
constexpr float VIEW_ASPECT = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// �r���[���ʂ̃A�X�y�N�g��	
constexpr float VIEW_NEAR_Z = 1.0f;											// �r���[���ʂ�NearZ�l
constexpr float VIEW_FAR_Z = 1000.0f;										// �r���[���ʂ�FarZ�l
constexpr float VALUE_MOVE_CAMERA = 2.0f;									// �J�����̈ړ���
constexpr float VALUE_ROTATE_CAMERA = DirectX::XM_PI * 0.01f;				// �J�����̉�]��

constexpr float CAM_POS_P_X = 0.0f;			// �J�����̎��_�����ʒu(X���W)
constexpr float CAM_POS_P_Y = 100.0f;		// �J�����̎��_�����ʒu(Y���W)
constexpr float CAM_POS_P_Z = -200.0f;		// �J�����̎��_�����ʒu(Z���W)
constexpr float CAM_POS_LOOKAT_X = 0.0f;	// �J�����̒����_�����ʒu(X���W)
constexpr float CAM_POS_LOOKAT_Y = 0.0f;	// �J�����̒����_�����ʒu(Y���W)
constexpr float CAM_POS_LOOKAT_Z = 0.0f;	// �J�����̒����_�����ʒu(Z���W)

constexpr float	INTERVAL_CAMERA_R = 12.5f;		// ���f���̎����̐�܂ł̋���
constexpr float	RATE_CHASE_CAMERA_P = 0.30f;	// �J�����̎��_�ւ̕␳�W��
constexpr float	RATE_CHASE_CAMERA_R = 0.20f;	// �J�����̒����_�ւ̕␳�W��
constexpr float	INTERVAL_CAMERA_R_MOVE = 20.5f;	// ���f���̎����̐�܂ł̋���

constexpr float	CHASE_HEIGHT_P = 200.0f;		// �ǐՎ��̎��_�̍���
constexpr float	CHASE_HEIGHT_R = 10.0f;			// �ǐՎ��̒����_�̍���

// static �ϐ�
DirectX::SimpleMath::Vector3	Camera::m_Position = DirectX::SimpleMath::Vector3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);//�J�����|�V�F�b�g
DirectX::SimpleMath::Vector3	Camera::m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);					 //�J������]
DirectX::SimpleMath::Vector3	Camera::m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);;						 //�J�����T�C�Y

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

	// �J�����ƒ����_�̋��������߂�
	m_TargetLength = (m_Position - m_Target).Length();

	// �ǔ����鎞�̈ʒu�i�����j
	m_HeightCameraP = CHASE_HEIGHT_P;

	// �ǔ����鎞�̒����_�̈ʒu�i�����j
	m_HeightCameraR = CHASE_HEIGHT_R;
}

void Camera::Uninit()
{

}

void Camera::Update()
{
	DirectX::XMFLOAT3	posModel;				// ���f�����W
	DirectX::XMFLOAT3	rotModel;				// ���f���p��
	DirectX::XMFLOAT3	moveModel;				// ���f���ړ���

	enum angle
	{
		FPS,
		TPS,
		Chase
	};

	float		fIntervalCamera;				// �J�����ƃ��f���̋���
	float		fLengthMove;					// �ړ���
	float		fRateChaseCameraPos;			// �J�����ʒu�̈ړ���
	float		fRateChaseCameraLookat;			// �����_�̈ړ���

	posModel = GetPositionPlayer();				// �v���C���̈ʒu���擾
	rotModel = GetRotationPlayer();				// �v���C���̌����擾
	moveModel = GetMovePlayer();				// �v���C���̈ړ��ʎ擾

	DirectX::XMFLOAT3 scale(1, 1, 1);
	DirectX::XMFLOAT3 trans(0, 0, 0);

	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		scale,
		rotModel,
		trans);

	//z�����o��
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

	// ���f�����ړ������H
	if (moveModel.x < -0.05f || moveModel.x > 0.05f
		|| moveModel.z < -0.05f || moveModel.z > 0.05f)
	{
		fIntervalCamera = INTERVAL_CAMERA_R + sqrtf(moveModel.x * moveModel.x + moveModel.z * moveModel.z) * 10.0f;
		fRateChaseCameraLookat = 0.10f;
		fRateChaseCameraPos = 0.30f;
	}
	else
	{
		// ���f������~��
		fIntervalCamera = INTERVAL_CAMERA_R + sqrtf(moveModel.x * moveModel.x + moveModel.z * moveModel.z) * 6.0f;
		fRateChaseCameraLookat = 0.1f;
		fRateChaseCameraPos = 0.30f;
	}
	fLengthMove = sqrtf(moveModel.x * moveModel.x + moveModel.z * moveModel.z) * 6.0f;

	switch (m_AngleType)
	{
	case FPS:
		//�ǉ�FPS
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

		//// �J�����̐���
		//if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_Z))
		//{
		//	// ���_����u���v
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
		//	// ���_����u�E�v
		//	m_Rotation.y -= VALUE_ROTATE_CAMERA;
		//	if (m_Rotation.y < -DirectX::XM_PI)
		//	{
		//		m_Rotation.y += DirectX::XM_PI * 2.0f;
		//	}
		//	m_Position.x = m_Target.x - sinf(m_Rotation.y) * m_TargetLength;
		//	m_Position.z = m_Target.z - cosf(m_Rotation.y) * m_TargetLength;
		//}

		// �����_�̖ڕW�ʒu
		m_TargetDest.x = posModel.x -sin(rotModel.y) * fIntervalCamera;
		m_TargetDest.y = posModel.y - m_HeightCameraR;
		m_TargetDest.z = posModel.z -cos(rotModel.y) * fIntervalCamera;

		// ���_�̖ڕW�ʒu
		m_PositionDest.x = posModel.x -sinf(m_Rotation.y) * m_TargetLength - sin(rotModel.y) * fLengthMove;
		m_PositionDest.y = posModel.y + m_HeightCameraP;
		m_PositionDest.z = posModel.z - cosf(m_Rotation.y) * m_TargetLength - cos(rotModel.y) * fLengthMove;

		// �����_�̕␳
		m_Target.x += (m_TargetDest.x - m_Target.x) * fRateChaseCameraLookat;
		m_Target.y += (m_TargetDest.y - m_Target.y) * 0.10f;
		m_Target.z += (m_TargetDest.z - m_Target.z) * fRateChaseCameraLookat;
		
		// ���_�̕␳
		m_Position += (m_PositionDest - m_Position) * fRateChaseCameraPos;
		
		break;

	}
	// ���C�g
	LIGHT light{};
	light.Enable = true;
	light.Direction = Vector4(m_ViewMatrix._13, m_ViewMatrix._23, m_ViewMatrix._33, 0.0f);//���C�g�̏Ƃ炷����
	light.Direction.Normalize();
	light.Ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
	light.Diffuse = Color(1.5f, 1.5f, 1.5f, 1.0f);
	Renderer::SetLight(light);

}

void Camera::Draw()
{
	// �r���[�ϊ����쐬
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, m_Target, up);		// ����n�ɂ����@20230511 by suzuki.tomoki

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//�v���W�F�N�V�����s��̐���
	float fieldOfView = VIEW_ANGLE;    // ����p
	
	float aspectRatio = VIEW_ASPECT;	// �A�X�y�N�g��	
	float nearPlane = VIEW_NEAR_Z;       // �j�A�N���b�v
	float farPlane = VIEW_FAR_Z;      // �t�@�[�N���b�v

	//�v���W�F�N�V�����s��̐���
	Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// ����n�ɂ����@20230511 by suzuki.tomoki


	Renderer::SetProjectionMatrix(&projectionMatrix);
}

void Camera::Draw2D() {
	Matrix projection;
	projection = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,
		static_cast<float>(SCREEN_WIDTH),//��ʂ̍ŏ��w
		static_cast<float>(SCREEN_HEIGHT),//��ʂ̍ŏ��x
		0.0f,//��ʂ̍ő�x
		0.0f,
		1.0f);

	Renderer::SetProjectionMatrix(&projection);

	//���[���h�ϊ��s��Z�b�g
	Matrix world;
	world = Matrix::Identity;//�P�ʍs��ɂ���
	Renderer::SetWorldMatrix(&world);

	//�r���[�ϊ��s��Z�b�g
	Matrix view;
	view = Matrix::Identity;

	Renderer::SetViewMatrix(&view);

}