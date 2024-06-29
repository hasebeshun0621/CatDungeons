#include	<SimpleMath.h>
#include	"Collision.h"
#include	"CBoundingBox.h"

using namespace DirectX::SimpleMath;

// ���E���̓����蔻��
bool HitCheckSphere(const BoundingSphere& p1, const BoundingSphere& p2){

	double centerlength = (p1.Center - p2.Center).Length();
	double radiuslength = (p1.Radius + p2.Radius);

	if (centerlength <= radiuslength) {
		return true;
	}

	return false;
}

// �O�p�`�̓������ǂ����𔻒肷��
bool CheckInTriangle(
	const Vector3& a,
	const Vector3& b,
	const Vector3& c,
	const Vector3& p) {

	Vector3	ab, bc, ca;			// �R�ӂ̃x�N�g��
	Vector3	ap, bp, cp;			// �����̓_�Ƃ̃x�N�g��
	Vector3	normal;				// �R�p�`�̖@���x�N�g��
	Vector3	n1, n2, n3;			// �R�ӂƓ����̓_�Ƃ̖@���x�N�g��

	ab = b - a;

	bc = c - b;

	ca = a - c;

	ap = p - a;

	bp = p - b;

	cp = p - c;

	normal = ab.Cross(bc);

	n1 = ab.Cross(ap);

	n2 = bc.Cross(bp);

	n3 = ca.Cross(cp);

	float dot = n1.Dot(normal);
	if (dot < 0) return false;	// �ׂ��p�x���݊p

	dot = n2.Dot(normal);
	if (dot < 0) return false;	// �ׂ��p�x���݊p

	dot = n3.Dot(normal);
	if (dot < 0) return false;	// �ׂ��p�x���݊p

	return true;
}

// ���C�ƒ����̌�������
bool LinetoPlaneCross(
	const Plane& plane,			//���ʂ̕�����
	const Vector3& p0,			//�����̋N�_
	const Vector3& wv,			//�����̕����x�N�g��
	float& t,					// ��_�ʒu���	
	Vector3& ans)				//��_���W
{

	Vector3 normal;				// ���ʂ̖@���x�N�g���i���K���ς݁j
	normal.x = plane.x;
	normal.y = plane.y;
	normal.z = plane.z;

	float dot;			// ����
	
	dot = wv.Dot(normal);
	// ���s�`�F�b�N(���ς��v�Z����)
	if (fabsf(dot) < FLT_EPSILON) {
		//		MessageBox(nullptr, "���s�ł��B", "���s�ł��B", MB_OK);

		printf("���s�ł� \n");
		return false;
	}

	// �����ɂ������߂鎮������
	float dot2;			// ���q
	dot2 = p0.Dot(normal);

	t = -(dot2 + plane.w) / dot;

	ans.x = p0.x + wv.x * t;
	ans.y = p0.y + wv.y * t;
	ans.z = p0.z + wv.z * t;
	return true;
}

// OBB�̓����蔻����s��
bool HitCheckOBB(
	const BoundingBoxOBB& obbA, 
	const BoundingBoxOBB& obbB) {

	//������
	Vector3 vecSeparate;

	// 2�̃I�u�W�F�N�g�����񂾃x�N�g�����v�Z
	Vector3 vecDistance;
	vecDistance = obbB.worldcenter - obbA.worldcenter;

	bool sts;	// �߂�l

	// OBB-A�����X�g
	const Vector3* OBB_A_Axis[3] = {
			&obbA.axisX,
			&obbA.axisY,
			&obbA.axisZ
	};

	// OBB-B�����X�g
	const Vector3* OBB_B_Axis[3] = {
			&obbB.axisX,
			&obbB.axisY,
			&obbB.axisZ
	};

	// OBB-A�̂R���𕪗����ɂ��ă`�F�b�N
	for (int i = 0; i < 3; i++) {

		Vector3 vecSeparate = *OBB_A_Axis[i];

		sts = CBoundingBox::CompareLength(
			obbA,				// OBB-A
			obbB,				// OBB-B
			vecSeparate,		// ������
			vecDistance);		// �Q�̃I�u�W�F�N�g�����ԃx�N�g��
		if (sts == false) {
			return false;
		}
	}

	// OBB-B�̂R���𕪗����ɂ��ă`�F�b�N
	for (int i = 0; i < 3; i++) {
		Vector3 vecSeparate = *OBB_B_Axis[i];

		sts = CBoundingBox::CompareLength(
			obbA,				// OBB-A
			obbB,				// OBB-B
			vecSeparate,		// ������
			vecDistance);		// �Q�̃I�u�W�F�N�g�����ԃx�N�g��
		if (sts == false) {
			return false;
		}
	}

	// �O�ς𕪗����Ƃ��Čv�Z
	for (int p1 = 0; p1 < 3; p1++) {
		for (int p2 = 0; p2 < 3; p2++) {

			Vector3 obbAaxis = *OBB_A_Axis[p1];
			Vector3 obbBaxis = *OBB_B_Axis[p2];

			Vector3	crossseparate;			// �O�σx�N�g��
			crossseparate = obbAaxis.Cross(obbBaxis);

			sts = CBoundingBox::CompareLength(
				obbA,				// OBB-A
				obbB,				// OBB-B
				crossseparate,		// ������
				vecDistance);		// �Q�̃I�u�W�F�N�g�����ԃx�N�g��
			if (sts == false) {
				return false;
			}
		}
	}

	return true;
}

