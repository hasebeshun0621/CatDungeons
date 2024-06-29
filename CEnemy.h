//#pragma once
//#include	<SimpleMath.h>
//#include	"CShader.h"
//#include	"CStaticMesh.h"
//#include	"CStaticMeshRenderer.h"
//#include	"Player.h"
//#include	"Field.h"
//
//class Enemy {
//	const	float RATE_ROTATE_ENEMY = 0.05f;			// ��]�����W��
//
//	 �G�l�~�[�̏��(S,R,T)
//	DirectX::SimpleMath::Vector3	m_position = { 0.0f,0.0f,0.0f };		// �ʒu
//	DirectX::SimpleMath::Vector3	m_rotation = { 0.0f,0.0f,0.0f };		// �p��
//	DirectX::SimpleMath::Vector3	m_scale = { 1.0f,1.0f,1.0f };			// �g�k
//
//	DirectX::SimpleMath::Vector3	m_move = { 0.0f,0.0f,0.0f };			// �ړ���
//
//	DirectX::SimpleMath::Vector3	m_destrot = { 0.0f,0.0f,0.0f };			// �ڕW��]�p�x
//
//	DirectX::SimpleMath::Matrix		m_mtx;
//
//	int m_fcnt = 0;//���˗p�J�E���^�[
//	int m_fullcnt = 0;//�e���ˊԊu
//
//	 �`��ׂ̈ɕK�v�ȏ��
//	 �g�p����V�F�[�_�[
//	CShader* m_shader = nullptr;
//	 �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
//	CStaticMesh* m_staticmesh = nullptr;
//	 ���b�V�������_���[
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
//		 �v���C���̍��W���擾
//		DirectX::SimpleMath::Vector3 playerpos = GetPositionPlayer();
//
//		 atan2���g�p���Ċp�x�����߂�
//		m_destrot.y = atan2f(-(playerpos.x - m_position.x), -(playerpos.z - m_position.z));
//
//		 ���݂̌����Ƃ̍������v�Z����
//		float fDiffRotY = m_destrot.y - m_rotation.y;
//
//		 �␳�i�|�P�W�O�`�P�W�O�͈̔́j
//		if (fDiffRotY > DirectX::XM_PI)
//		{
//			fDiffRotY -= DirectX::XM_PI * 2.0f;
//		}
//		if (fDiffRotY < -DirectX::XM_PI)
//		{
//			fDiffRotY += DirectX::XM_PI * 2.0f;
//		}
//
//		 ��]�p�x�v�Z
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
//	 �`��
//	void Draw() {
//
//		 �f�o�C�X�R���e�L�X�g�擾
//		ID3D11DeviceContext* devicecontext;
//		devicecontext = Renderer::GetDeviceContext();
//
//		 ���[���h�ϊ��s�񐶐�
//		DirectX::SimpleMath::Matrix mtx;
//		DX11MakeWorldMatrixRadian(
//			mtx,
//			m_scale,							// �X�P�[��
//			m_rotation,							// �p��
//			m_position);						// �ʒu
//
//		m_mtx = mtx;
//
//		 GPU�ɍs����Z�b�g����
//		Renderer::SetWorldMatrix(&mtx);
//
//		 �V�F�[�_�[��GPU�ɃZ�b�g
//		m_shader->SetGPU();
//
//		 ���f���`��
//		m_staticmeshrenderer->Draw();
//	}
//};