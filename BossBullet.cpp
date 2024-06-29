#include	"BossBullet.h"

using namespace DirectX::SimpleMath;

//�@�{�X�o���b�g�\����
BossBullet g_bossbullet[BulletEnemy_Max];

//�m�b�N�o�b�N�A�I���I�t
static bool kbsts = false;

// �V�F�[�_�[
static CShader		g_shader;

// �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
static CStaticMesh g_staticmesh;

// ���b�V�������_���[
static CStaticMeshRenderer g_staticmeshrenderer;

// ���b�V�������_��
static CMeshRenderer g_meshrenderer;

// �}�e���A��
static CMaterial g_material;

// �{�b�N�X���b�V��
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb;

// �{�X�o���b�gOBB�擾
BoundingBoxOBB GetBossBulletOBB(int idx) {
	Matrix mtx = GetMtxBossBullet(idx);
	return g_obb.MakeOBB(mtx, g_bossbullet[idx].scale);
}

//�@�{�X�o���b�g�_���[�W�擾
int GetBossBulletDame(int idx) {
	return g_bossbullet[idx].Dame;
}

//�@�{�X�o���b�g�}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxBossBullet(int idx) {

	Matrix bmtx;
	DX11MakeWorldMatrixRadian(
		bmtx,
		g_bossbullet[idx].scale,
		g_bossbullet[idx].rotation,
		g_bossbullet[idx].position);

	return bmtx;
}

//�@�{�X�o���b�g������
BossBullet InitializBossBullet() {

	BossBullet obj;
	obj.sts = -1;								//���g�p-1�@�g�p���P
	obj.position = { 0,0,0 };					//�ʒu
	obj.rotation = { 0,0,0 };					//��]
	obj.scale = { 9.0f,11.0f,10.0f };			//�T�C�Y
	obj.vector = { 0.0f,0.0f,0.0f };			//�x�N�g���擾
	obj.Speed = 4.0f;							//�e��
	obj.Dame = ENEMYBULLETDAME_BOSS;

	return obj;
}

//�@�{�X�o���b�g��������
void InitBossBullet() {
	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// ���_�V�F�[�_
		"shader/vertexLightingPS.hlsl");				// �s�N�Z���V�F�[�_

	// ���f���t�@�C����
	std::string filename[] = {
		u8"assets\\model\\ya.obj",
	};

	//�m�b�N�o�b�N�A�I���I�t
	kbsts = false;

	// ���b�V�������i�W�I���g���f�[�^�j
	g_staticmesh.Init(filename[0]);

	// �`��ׂ̈̃f�[�^����
	g_staticmeshrenderer.Init(g_staticmesh);

	for (int i = 0; i < BossBullet_Max; i++) {
		//�G�l�~�[������
		g_bossbullet[i] = InitializBossBullet();
	}

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[//
	// OBB�����߂�
	g_obb.Caliculate(g_staticmesh.GetVertices());

	// �����b�V���𐶐�����
	Vector3 diff = g_obb.GetMax() - g_obb.GetMin();
	g_box.Init(
		diff.x,
		diff.y,
		diff.z,
		Color(1, 1, 1, 1));
	g_meshrenderer.Init(g_box);

	// OBB�`��p�}�e���A��
	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 0, 0.3f);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.TextureEnable = FALSE;
	g_material.Init(mtrl);

}

//�@�{�X�o���b�g�X�V����
void UpdateBossBullet() {

	MoveBossBullet();
	HitCheckPlayer_Boss();

}

//�@�{�X�o���b�g�`�揈��
void DrawBossBullet() {
	
	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	for (int i = 0; i < BossBullet_Max; i++) {
		if (g_bossbullet[i].sts == 1) {
			// ���[���h�ϊ��s�񐶐�
			Matrix mtx;
			DX11MakeWorldMatrixRadian(
				mtx,
				g_bossbullet[i].scale,							// �X�P�[��
				g_bossbullet[i].rotation,						// �p��
				g_bossbullet[i].position);						// �ʒu

			// GPU�ɍs����Z�b�g����
			Renderer::SetWorldMatrix(&mtx);

			// �V�F�[�_�[��GPU�ɃZ�b�g
			g_shader.SetGPU();
			// ���f���`��
			g_staticmeshrenderer.Draw();

			//-------------------------------------//���`��
			////OBB��񐶐�
			////���E�{�b�N�X�`��
			//mtx = g_obb[g_enemybullet[i].BulletType].MakeWorldMtx(g_enemybullet[i].scale, mtx);
			//Renderer::SetWorldMatrix(&mtx);
			//// �}�e���A�����Z�b�g
			//g_material.SetGPU();
			//g_meshrenderer[g_enemybullet[i].BulletType].Draw();
		}
	}

}

//�@�{�X�o���b�g�I������
void ExitBossBullet() {

	g_staticmeshrenderer.Uninit();

}

//�@�{�X�o���b�g���ˏ���
void FireBossBullet(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot) {

	//�v���C���[�|�W�V�����擾
	DirectX::XMFLOAT3 playerpos = GetPositionPlayer();

	for (int i = 0; i < BossBullet_Max; i++) {
		//�g�p�ς݃o���b�g���
		if (g_bossbullet[i].sts == 1) {

		}

		//���g�p�o���b�g�擾->�g�p
		if (g_bossbullet[i].sts == -1) {
			//���̎Εӂ����߂�
			//���̂����ΕӂŊ���A�x��
			//���߂���̂̓x�N�g�����P�ŕ\�����������normalize�ƌ���

			//�o���b�g���G�l�~�[�̍��W���擾
			g_bossbullet[i].position = pos;

			//�G�l�~�[��I�Ԗڂ̔��˕����擾
			g_bossbullet[i].rotation = rot;

			//�v���C���[�ƃG�l�~�[�̋���(��)
			DirectX::XMFLOAT3 sa = playerpos - g_bossbullet[i].position;

			//�x�N�g���̒���
			float v = sqrt((sa.x * sa.x) + (sa.z * sa.z));

			//�P�ʃx�N�g��
			g_bossbullet[i].vector = DirectX::XMFLOAT3(sa.x / v, sa.y, sa.z / v);

			g_bossbullet[i].sts = 1;
			break;//���g�p���������for����o��
		}
	}

}

//�@�{�X�o���b�g�ړ�
void MoveBossBullet() {

	for (int i = 0; i < BossBullet_Max; i++) {
		if (g_bossbullet[i].sts == 1) {
			//�ړ��X�V
			g_bossbullet[i].position.x += g_bossbullet[i].vector.x * g_bossbullet[i].Speed;
			g_bossbullet[i].position.z += g_bossbullet[i].vector.z * g_bossbullet[i].Speed;

		}

		if (MapOutCheck(g_bossbullet[i].position)) {
			g_bossbullet[i].sts = -1;

		}
	}

}

//�@�{�X�o���b�g�ƃv���C���[�̓����蔻��
void HitCheckPlayer_Boss() {

	static	int ebnum = 0;
	if (GetPlayerHitSts()) {
		for (int i = 0; i < BossBullet_Max; i++) {
			bool sts = HitCheckOBB(GetPlayerOBB(), GetBossBulletOBB(i));
			if (sts && g_bossbullet[i].sts == 1) {
				g_bossbullet[i].sts = -1;
				//�e�̃_���[�W���v���C���[�̗̑͂����炷
				PlayerHPDOWN(GetBossBulletDame(i));
				MakeDameParticle(g_bossbullet[i].position);
				SetHitStop(5);

				ebnum = i;
				kbsts = true;
			}
		}
	}
	if (kbsts && HitStop()) {
		kbsts = PlayerKnockBack(ebnum,2);
	}

}
