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
	//�J������������
	static void Init();
	//�J�����X�V����
	static void Update();
	//�J�����`�揈��
	static void Draw();
	//�J����2D�`�揈��
	static void Draw2D();
	//�J�����I������
	static void Uninit();

	// �J�����̉�]���擾
	static DirectX::SimpleMath::Vector3 GetRotCamera() {
		return m_Rotation;
	}

	// �r���[�}�g���b�N�X���擾
	static DirectX::SimpleMath::Matrix GetViewMtx() {
		return m_ViewMatrix;
	}
};