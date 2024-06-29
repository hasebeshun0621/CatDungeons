#pragma once
#include	<SimpleMath.h>

// 境界球
struct BoundingSphere {
	DirectX::SimpleMath::Vector3 Center;
	float Radius;
};

// 境界ボックスAABB
struct BoundingBoxAABB {
	DirectX::SimpleMath::Vector3 min;
	DirectX::SimpleMath::Vector3 max;
};

// 境界ボックスOBB
struct BoundingBoxOBB : public BoundingBoxAABB{
	DirectX::SimpleMath::Vector3 center;
	DirectX::SimpleMath::Vector3 worldcenter;

	// 中心軸（X軸、Y軸、Z軸）
	DirectX::SimpleMath::Vector3 axisX;
	DirectX::SimpleMath::Vector3 axisY;
	DirectX::SimpleMath::Vector3 axisZ;

	// BOXサイズ
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
	const DirectX::SimpleMath::Plane& plane,	//	平面の方程式
	const DirectX::SimpleMath::Vector3& p0,		//	直線の起点
	const DirectX::SimpleMath::Vector3& wv,		//	直線の方向ベクトル
	float& t,									//	交点位置情報	
	DirectX::SimpleMath::Vector3& ans);			//	交点座標


// ローカル座標で生成したOBBをワールド空間に変換
BoundingBoxOBB MakeOBB(
	const BoundingBoxOBB& inobb, 
	DirectX::SimpleMath::Matrix worldmtx, 
	DirectX::SimpleMath::Vector3 scale);

// OBBの当たり判定
bool HitCheckOBB(
	const BoundingBoxOBB& obbA,
	const BoundingBoxOBB& obbB);