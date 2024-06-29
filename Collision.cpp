#include	<SimpleMath.h>
#include	"Collision.h"
#include	"CBoundingBox.h"

using namespace DirectX::SimpleMath;

// 境界球の当たり判定
bool HitCheckSphere(const BoundingSphere& p1, const BoundingSphere& p2){

	double centerlength = (p1.Center - p2.Center).Length();
	double radiuslength = (p1.Radius + p2.Radius);

	if (centerlength <= radiuslength) {
		return true;
	}

	return false;
}

// 三角形の内部かどうかを判定する
bool CheckInTriangle(
	const Vector3& a,
	const Vector3& b,
	const Vector3& c,
	const Vector3& p) {

	Vector3	ab, bc, ca;			// ３辺のベクトル
	Vector3	ap, bp, cp;			// 内部の点とのベクトル
	Vector3	normal;				// ３角形の法線ベクトル
	Vector3	n1, n2, n3;			// ３辺と内部の点との法線ベクトル

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
	if (dot < 0) return false;	// 為す角度が鈍角

	dot = n2.Dot(normal);
	if (dot < 0) return false;	// 為す角度が鈍角

	dot = n3.Dot(normal);
	if (dot < 0) return false;	// 為す角度が鈍角

	return true;
}

// レイと直線の交差判定
bool LinetoPlaneCross(
	const Plane& plane,			//平面の方程式
	const Vector3& p0,			//直線の起点
	const Vector3& wv,			//直線の方向ベクトル
	float& t,					// 交点位置情報	
	Vector3& ans)				//交点座標
{

	Vector3 normal;				// 平面の法線ベクトル（正規化済み）
	normal.x = plane.x;
	normal.y = plane.y;
	normal.z = plane.z;

	float dot;			// 分母
	
	dot = wv.Dot(normal);
	// 平行チェック(内積を計算する)
	if (fabsf(dot) < FLT_EPSILON) {
		//		MessageBox(nullptr, "平行です。", "平行です。", MB_OK);

		printf("平行です \n");
		return false;
	}

	// ここにｔを求める式を入れる
	float dot2;			// 分子
	dot2 = p0.Dot(normal);

	t = -(dot2 + plane.w) / dot;

	ans.x = p0.x + wv.x * t;
	ans.y = p0.y + wv.y * t;
	ans.z = p0.z + wv.z * t;
	return true;
}

// OBBの当たり判定を行う
bool HitCheckOBB(
	const BoundingBoxOBB& obbA, 
	const BoundingBoxOBB& obbB) {

	//分離軸
	Vector3 vecSeparate;

	// 2つのオブジェクトを結んだベクトルを計算
	Vector3 vecDistance;
	vecDistance = obbB.worldcenter - obbA.worldcenter;

	bool sts;	// 戻り値

	// OBB-A軸リスト
	const Vector3* OBB_A_Axis[3] = {
			&obbA.axisX,
			&obbA.axisY,
			&obbA.axisZ
	};

	// OBB-B軸リスト
	const Vector3* OBB_B_Axis[3] = {
			&obbB.axisX,
			&obbB.axisY,
			&obbB.axisZ
	};

	// OBB-Aの３軸を分離軸にしてチェック
	for (int i = 0; i < 3; i++) {

		Vector3 vecSeparate = *OBB_A_Axis[i];

		sts = CBoundingBox::CompareLength(
			obbA,				// OBB-A
			obbB,				// OBB-B
			vecSeparate,		// 分離軸
			vecDistance);		// ２つのオブジェクトを結ぶベクトル
		if (sts == false) {
			return false;
		}
	}

	// OBB-Bの３軸を分離軸にしてチェック
	for (int i = 0; i < 3; i++) {
		Vector3 vecSeparate = *OBB_B_Axis[i];

		sts = CBoundingBox::CompareLength(
			obbA,				// OBB-A
			obbB,				// OBB-B
			vecSeparate,		// 分離軸
			vecDistance);		// ２つのオブジェクトを結ぶベクトル
		if (sts == false) {
			return false;
		}
	}

	// 外積を分離軸として計算
	for (int p1 = 0; p1 < 3; p1++) {
		for (int p2 = 0; p2 < 3; p2++) {

			Vector3 obbAaxis = *OBB_A_Axis[p1];
			Vector3 obbBaxis = *OBB_B_Axis[p2];

			Vector3	crossseparate;			// 外積ベクトル
			crossseparate = obbAaxis.Cross(obbBaxis);

			sts = CBoundingBox::CompareLength(
				obbA,				// OBB-A
				obbB,				// OBB-B
				crossseparate,		// 分離軸
				vecDistance);		// ２つのオブジェクトを結ぶベクトル
			if (sts == false) {
				return false;
			}
		}
	}

	return true;
}

