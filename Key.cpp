#include "Key.h"

using namespace DirectX::SimpleMath;

//�@�J�M�\����
Key g_Key;

// �g�p����V�F�[�_�[
static CShader		g_shader;

// �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
static CStaticMesh g_staticmesh;

// ���b�V�������_���[
static CStaticMeshRenderer g_staticmeshrenderer;

//---------------------------------------------------//
//obb���b�V���A�V�F�[�_��`

// ���b�V�������_��
static CMeshRenderer g_meshrenderer;

// �}�e���A��
static CMaterial g_material;

// �{�b�N�X���b�V��
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb;

//�@�J�M�X�e�[�^�X�擾
int GetKeySts() {
	return g_Key.sts;
}

//�@�J�M�}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxKey() {

	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_Key.scale,
		g_Key.rotation,
		g_Key.position);

	return mtx;
}

//�@�J�MOBB�擾
BoundingBoxOBB GetKeyOBB() {
	Matrix mtx = GetMtxKey();
	return g_obb.MakeOBB(mtx, g_Key.scale);
}

//�@�J�M��������
void InitKey() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// ���_�V�F�[�_
		"shader/vertexLightingPS.hlsl");				// �s�N�Z���V�F�[�_

	// ���f���t�@�C����
	std::string filename[] = {
		u8"assets\\model\\Key.obj"
	};

	g_Key.sts = -1;

	// ���b�V�������i�W�I���g���f�[�^�j
	g_staticmesh.Init(filename[0]);

	// �`��ׂ̈̃f�[�^����
	g_staticmeshrenderer.Init(g_staticmesh);

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[//
	// OBB����
	g_obb.Caliculate(g_staticmesh.GetVertices());

	// OBB�`��p���b�V������
	Vector3 diff = g_obb.GetMax() - g_obb.GetMin();
	g_box.Init(diff.x, diff.y, diff.z, Color(1, 1, 1, 1));
	g_meshrenderer.Init(g_box);

	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 0.3f);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.TextureEnable = FALSE;
	g_material.Init(mtrl);

}

//�@�J�M�X�V����
void UpdateKey() {

	if (g_Key.sts == 1) {
		static float num = 0.5f;
		if (g_Key.position.y < 0 || g_Key.position.y > 20) {
			num *= -1;
		}
		g_Key.position.y += num;

		if (g_Key.position.y > 0) {
			g_Key.position.y-=2;
		}
		g_Key.rotation.y += 0.05f;
	}

}

//�@�J�M�`�揈��
void DrawKey() {
	
	if (g_Key.sts == 1) {
		// �f�o�C�X�R���e�L�X�g�擾
		ID3D11DeviceContext* devicecontext;
		devicecontext = Renderer::GetDeviceContext();

		// ���[���h�ϊ��s�񐶐�
		Matrix mtx;
		DX11MakeWorldMatrixRadian(
			mtx,
			g_Key.scale,							// �X�P�[��
			g_Key.rotation,							// �p��
			g_Key.position);						// �ʒu

		// GPU�ɍs����Z�b�g����
		Renderer::SetWorldMatrix(&mtx);

		// �V�F�[�_�[��GPU�ɃZ�b�g
		g_shader.SetGPU();

		// ���f���`��
		g_staticmeshrenderer.Draw();
		//-------------------------------------//���`��
		// ���E�{�b�N�X�`��
		//mtx = g_obb.MakeWorldMtx(g_Key.scale, mtx);
		//Renderer::SetWorldMatrix(&mtx);
		//g_material.SetGPU();
		//g_meshrenderer.Draw();
	}

}

//�@�J�M�I������
void ExitKey() {
	g_staticmeshrenderer.Uninit();
}

//�J�M�h���b�v
void KeyDrop(int idx) {

	//�G�l�~�[������ł��茮�������Ă���Ȃ�
	if (GetKeyEnemy(idx) &&GetStsEnemy(idx) == -1) {
		g_Key.position = GetEnemyPosition(idx);
		g_Key.sts = 1;
	}

}

//�@�J�M�����h���b�v
void FallKeyDrop(int idx) {

	//�G�l�~�[������ł��茮�������Ă���Ȃ�
	if (GetKeyEnemy(idx) && GetStsEnemy(idx) == -1) {
		g_Key.position.x = (rand() % (int)FieldX) - FieldX / 2;
		g_Key.position.y = 400.0f;
		g_Key.position.z = (rand() % (int)FieldZ) - FieldZ / 2;;
		g_Key.sts = 1;
	}

}

//�@�v���C���[�ƃJ�M�̓����蔻��
bool HitCheckKey() {

	bool sts = false;
	if (g_Key.sts == 1) {
		sts = HitCheckOBB(GetPlayerOBB(), GetKeyOBB());
	}

	return sts;
}


