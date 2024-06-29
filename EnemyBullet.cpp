#include	"EnemyBullet.h"

using namespace DirectX::SimpleMath;

//�@�G�l�~�[�o���b�g�\����
EnemyBullet g_enemybullet[BulletEnemy_Max];

//�m�b�N�o�b�N�A�I���I�t
static bool kbsts = false;

// �V�F�[�_�[
static CShader		g_shader;

// �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
static CStaticMesh g_staticmesh[EnemyBulletMod_Max];

// ���b�V�������_���[
static CStaticMeshRenderer g_staticmeshrenderer[EnemyBulletMod_Max];

//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[//

// ���b�V�������_��
static CMeshRenderer g_meshrenderer[EnemyBulletMod_Max];

// �}�e���A��
static CMaterial g_material;

// �{�b�N�X���b�V��
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb[EnemyBulletMod_Max];

//�@�G�l�~�[�o���b�g�}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxEnemyBullet(int idx) {

	Matrix bmtx;
	DX11MakeWorldMatrixRadian(
		bmtx,
		g_enemybullet[idx].scale,
		g_enemybullet[idx].rotation,
		g_enemybullet[idx].position);

	return bmtx;
}

// �G�l�~�[�o���b�gOBB�擾
BoundingBoxOBB GetEnemyBulletOBB(int idx) {
	Matrix mtx = GetMtxEnemyBullet(idx);
	return g_obb[g_enemybullet[idx].BulletType].MakeOBB(mtx, g_enemybullet[idx].scale);
}

//�@�G�l�~�[�o���b�g�̃^�C�v���擾
int GetEnemyBulletType(int idx) {
	return g_enemybullet[idx].BulletType;
}

//�@�G�l�~�[�o���b�g�̃X�s�[�h���擾
float GetEnemyBulletSpeed(int idx){
	return g_enemybullet[idx].Speed;
}

//�@�G�l�~�[�o���b�g�_���[�W�擾
int GetEnemyBulletDame(int idx) {
	return g_enemybullet[idx].Dame;
}

//�@�G�l�~�[�o���b�g�^�C�v�ʏ�����
EnemyBullet InitializBullet_Sura() {

	EnemyBullet obj;
	obj.sts = -1;
	obj.BulletType = 0;
	obj.position = { 0,0,0 };
	obj.rotation = { 0,0,0 };
	obj.scale = { 10.0f,10.0f,12.0f };
	obj.vector = { 0.0f,0.0f,0.0f };
	obj.Speed = 2.5f;
	obj.Dame = ENEMYBULLETDAME_Sura;

	return obj;
}

EnemyBullet InitializBullet_SuraLeaf() {

	EnemyBullet obj;
	obj.sts = -1;
	obj.BulletType = 1;
	obj.position = { 0,0,0 };
	obj.rotation = { 0,0,0 };
	obj.scale = { 9.0f,11.0f,10.0f };
	obj.vector = { 0.0f,0.0f,0.0f };
	obj.Speed = 2.0f;
	obj.Dame = ENEMYBULLETDAME_SuraLeaf;

	return obj;
}

//�@�G�l�~�[�o���b�g��������
void InitEnemyBullet() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// ���_�V�F�[�_
		"shader/vertexLightingPS.hlsl");				// �s�N�Z���V�F�[�_

	// ���f���t�@�C����
	std::string filename[] = {
		u8"assets\\model\\suraBullet.obj",
		u8"assets\\model\\leafBullet.obj",
	};
	
	//�m�b�N�o�b�N�A�I���I�t
	kbsts = false;

	for (int i = 0; i < EnemyBulletMod_Max; i++) {
		// ���b�V�������i�W�I���g���f�[�^�j
		g_staticmesh[i].Init(filename[i]);

		// �`��ׂ̈̃f�[�^����
		g_staticmeshrenderer[i].Init(g_staticmesh[i]);

		// OBB�����߂�
		g_obb[i].Caliculate(g_staticmesh[i].GetVertices());

		// �����b�V���𐶐�����
		Vector3 diff = g_obb[i].GetMax() - g_obb[i].GetMin();
		g_box.Init(
			diff.x,
			diff.y,
			diff.z,
			Color(1, 1, 1, 1));

		g_meshrenderer[i].Init(g_box);
	}

	for (int i = 0; i < BulletEnemy_Max; i++) {
		static int cnt = 0;
		switch (cnt)
		{
		case 0:
			//�G�l�~�[������
			g_enemybullet[i] = InitializBullet_Sura();
			cnt++;
			break;
		case 1:
			//�G�l�~�[������
			g_enemybullet[i] = InitializBullet_SuraLeaf();
			cnt = 0;
			break;
		}
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[//
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

//�@�G�l�~�[�o���b�g�X�V����
void UpdateEnemyBullet() {

	MoveEnemyBullet();
	HitCheckPlayer_Enemy();

}

//�@�G�l�~�[�o���b�g�`�揈��
void DrawEnemyBullet() {

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	for (int i = 0; i < BulletEnemy_Max; i++) {
		if (g_enemybullet[i].sts == 1) {
			// ���[���h�ϊ��s�񐶐�
			Matrix mtx;
			DX11MakeWorldMatrixRadian(
				mtx,
				g_enemybullet[i].scale,							// �X�P�[��
				g_enemybullet[i].rotation,						// �p��
				g_enemybullet[i].position);						// �ʒu

			// GPU�ɍs����Z�b�g����
			Renderer::SetWorldMatrix(&mtx);

			// �V�F�[�_�[��GPU�ɃZ�b�g
			g_shader.SetGPU();
			// ���f���`��
			g_staticmeshrenderer[g_enemybullet[i].BulletType].Draw();

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

//�@�G�l�~�[�o���b�g�I������
void ExitEnemyBullet() {

	for (int i = 0; i < EnemyBulletMod_Max; i++) {
		g_staticmeshrenderer[i].Uninit();
	}

}

//�@�G�l�~�[�o���b�g���ˏ���
void FireEnemyBullet(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT3 rot,int type) {
	
	//�v���C���[�|�W�V�����擾
	DirectX::XMFLOAT3 playerpos = GetPositionPlayer();

	for (int i = 0; i < BulletEnemy_Max; i++) {
		//�G�l�~�[�̃^�C�v�ƒe�̃^�C�v����v���Ă���e���擾
		if (type == g_enemybullet[i].BulletType) {
			//�g�p�ς݃o���b�g���
			if (g_enemybullet[i].sts == 1) {

			}

			//���g�p�o���b�g�擾->�g�p
			if (g_enemybullet[i].sts == -1) {
				//���̎Εӂ����߂�
				//���̂����ΕӂŊ���A�x��
				//���߂���̂̓x�N�g�����P�ŕ\�����������normalize�ƌ���

				//�o���b�g���G�l�~�[�̍��W���擾
				g_enemybullet[i].position = pos;

				//�G�l�~�[��I�Ԗڂ̔��˕����擾
				g_enemybullet[i].rotation = rot;

				//�v���C���[�ƃG�l�~�[�̋���(��)
				DirectX::XMFLOAT3 sa = playerpos - g_enemybullet[i].position;

				//�x�N�g���̒���
				float v = sqrt((sa.x * sa.x) + (sa.z * sa.z));

				//�P�ʃx�N�g��
				g_enemybullet[i].vector = DirectX::XMFLOAT3(sa.x / v, sa.y, sa.z / v);

				g_enemybullet[i].sts = 1;
				break;//���g�p���������for����o��
			}
		}
	}

}

//�@�G�l�~�[�o���b�g�ړ�
void MoveEnemyBullet() {

	for (int i = 0; i < BulletEnemy_Max; i++) {
		if (g_enemybullet[i].sts == 1) {
			//�ړ��X�V
			g_enemybullet[i].position.x += g_enemybullet[i].vector.x * g_enemybullet[i].Speed;
			g_enemybullet[i].position.z += g_enemybullet[i].vector.z * g_enemybullet[i].Speed;
		}

		if (MapOutCheck(g_enemybullet[i].position)) {
			g_enemybullet[i].sts = -1;
		}
	}

}

//�@�v���C���[�ƃG�l�~�[�o���b�g�̓����蔻��
void HitCheckPlayer_Enemy() {
	
	static	int ebnum = 0;
	if (GetPlayerHitSts()) {
		for (int i = 0; i < BulletEnemy_Max; i++) {
			bool sts = HitCheckOBB(GetPlayerOBB(), GetEnemyBulletOBB(i));
			if (sts && g_enemybullet[i].sts == 1) {
				g_enemybullet[i].sts = -1;
				//�e�̃_���[�W���v���C���[�̗̑͂����炷
				PlayerHPDOWN(GetEnemyBulletDame(i));
				SetHitStop(4);
				MakeDameParticle(g_enemybullet[i].position);
				ebnum = i;
				kbsts = true;
			}
		}
	}
	if (kbsts&&HitStop()) {
		kbsts = PlayerKnockBack(ebnum,1);
	}

}

//�@�o���b�g�ƃl�b�g�̓����蔻��
bool HitCheckNet() {
	
	for (int i = 0; i < BulletEnemy_Max; i++) {
		bool sts = HitCheckOBB(GetPointerOBB(),GetEnemyBulletOBB(i));
		//�q�b�g���Ƀ{�^�����������Ȃ�
		if (sts && g_enemybullet[i].sts == 1 && GetStsPointer() == 1) {
			g_enemybullet[i].sts = -1;

			CatchCntUp(g_enemybullet[i].BulletType);

			SetHitStop(10);//�q�b�g�X�g�b�v�ݒ�

			MakeCatchParticle(g_enemybullet[i].position);
			MakeBulletCatchParticle(g_enemybullet[i].position);

			PlaySound(CatchSE);

			return true;
		}
	}

	return false;
}