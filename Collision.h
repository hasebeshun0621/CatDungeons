#pragma once
#include	<SimpleMath.h>

// ���E��
struct BoundingSphere {
	DirectX::SimpleMath::Vector3 Center;
	float Radius;
};

// ���E�{�b�N�XAABB
struct BoundingBoxAABB {
	DirectX::SimpleMath::Vector3 min;
	DirectX::SimpleMath::Vector3 max;
};

// ���E�{�b�N�XOBB
struct BoundingBoxOBB : public BoundingBoxAABB{
	DirectX::SimpleMath::Vector3 center;
	DirectX::SimpleMath::Vector3 worldcenter;

	// ���S���iX���AY���AZ���j
	DirectX::SimpleMath::Vector3 axisX;
	DirectX::SimpleMath::Vector3 axisY;
	DirectX::SimpleMath::Vector3 axisZ;

	// BOX�T�C�Y
	float	lengthx;
	float	lengthy;
	float	lengthz;
};

bool HitCheckSphere(const BoundingSphere& p1, const BoundingSphere& p2);

bool CheckInTriangle(
	const DirectX::SimpleMath::Vector3& a,
	const DirectX::SimpleMath::Vector3& b,
	const DirectX::SimpleMath::Vector3& c,
	const DirectX::SimpleMath::Vector3& p);

bool LinetoPlaneCross(
	const DirectX::SimpleMath::Plane& plane,	//	���ʂ̕�����
	const DirectX::SimpleMath::Vector3& p0,		//	�����̋N�_
	const DirectX::SimpleMath::Vector3& wv,		//	�����̕����x�N�g��
	float& t,									//	��_�ʒu���	
	DirectX::SimpleMath::Vector3& ans);			//	��_���W


// ���[�J�����W�Ő�������OBB�����[���h��Ԃɕϊ�
BoundingBoxOBB MakeOBB(
	const BoundingBoxOBB& inobb, 
	DirectX::SimpleMath::Matrix worldmtx, 
	DirectX::SimpleMath::Vector3 scale);

// OBB�̓����蔻��
bool HitCheckOBB(
	const BoundingBoxOBB& obbA,
	const BoundingBoxOBB& obbB);