#pragma once
#include	<SimpleMath.h>
#include	<algorithm>
#include	<iostream>
#include	"renderer.h"
#include	"Collision.h"

class CBoundingSphere
{
	DirectX::SimpleMath::Vector3 m_center;//�������W
	float m_radius;

	DirectX::SimpleMath::Vector3 m_max;//�ő�lAABB
	DirectX::SimpleMath::Vector3 m_min;//�ŏ��lAABB

	// ���[���h���W���v�Z����i�ΏۃI�u�W�F�N�g�̍s��iRTS�j���������j
	DirectX::SimpleMath::Vector3 CalcWorldPos(DirectX::SimpleMath::Matrix mtx) {
		return DirectX::SimpleMath::Vector3::Transform(m_center, mtx);
	}
public:
	//BS���𐶐�����
	void Caliculate(const std::vector<VERTEX_3D>& vertices) {
		std::vector<float> lengthlist;

		DirectX::SimpleMath::Vector3 max;
		DirectX::SimpleMath::Vector3 min;

		//�ő�ƍŏ������߂�
		min = max = vertices[0].Position;

		for (auto& v : vertices) {
			if (min.x > v.Position.x)min.x = v.Position.x;
			if (min.y > v.Position.y)min.y = v.Position.y;
			if (min.z > v.Position.z)min.z = v.Position.z;

			if (max.x < v.Position.x)max.x = v.Position.x;
			if (max.y < v.Position.y)max.y = v.Position.y;
			if (max.z < v.Position.z)max.z = v.Position.z;

		}
		m_min = min;
		m_max = max;

		std::cout << m_min.x << "," << m_min.y << "," << m_min.z << std::endl;
		std::cout << m_max.x << "," << m_max.y << "," << m_max.z << std::endl;

		//���S�����߂�
		DirectX::SimpleMath::Vector3 center;
		center = (max + min) * 0.5f;
		m_center = center;

		//���a�����߂�
		for (auto& v : vertices) {
			float length;
			length = (v.Position - center).Length();
			lengthlist.emplace_back(length);

		}

		//�ő�̒��������߂�
		auto iter = std::max_element(lengthlist.begin(), lengthlist.end());
		m_radius = *iter;
	}

	//���a��߂�
	float GetRadius() {
		return m_radius;
	}
	//���S���W��߂�
	DirectX::SimpleMath::Vector3 GetCenter() {
		return m_center;
	}

	//BS��񐶐�
	BoundingSphere MakeBS(
		DirectX::SimpleMath::Matrix mtx,
		DirectX::SimpleMath::Vector3 scale) 
	{
		BoundingSphere bs;
		//���S���W�v�Z
		DirectX::SimpleMath::Vector3 wpos = CalcWorldPos(mtx);
		bs.Center = wpos;
	
		//���a���Čv�Z�i�g�k������ׁj
		float maxsclae = std::max(std::max(scale.x, scale.y), scale.z);
		bs.Radius = m_radius * maxsclae;
		
		return bs;
	}

	//BS�`��p�̃��[���h�ϊ��s��𐶐�����
	DirectX::SimpleMath::Matrix MakeWorldMtx(
		DirectX::SimpleMath::Vector3 scale,
		DirectX::SimpleMath::Matrix wmtx) {
		//�P����3D�W�I���g����񂩂�BS�����쐬���Ă���
		//�i�g�k���l������Ă��Ȃ��j
		float smax;
		smax = std::max(std::max(scale.x, scale.y), scale.z);

		//�ő�̊g�嗦�ōs�񐶐�
		DirectX::SimpleMath::Matrix mtx = DirectX::SimpleMath::Matrix::CreateScale(smax, smax, smax);

		DirectX::SimpleMath::Vector3 bspos = CalcWorldPos(wmtx);

		//�o�E���f�B���O�X�t�B�A�p�̃��[���h�ϊ��s��ɂ���i���S������Ă���̂ŏC���j
		mtx._41 = bspos.x;
		mtx._42 = bspos.y;
		mtx._43 = bspos.z;

		return mtx;
	}
};
