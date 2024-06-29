#include	"PlayerBullet.h"

using namespace DirectX::SimpleMath;

//�@�v���C���[�o���b�g�\����
PlayerBullet g_playerbullet[PlayerBullet_Max];

// �V�F�[�_�[
static CShader		g_shader;

// �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
static CStaticMesh g_staticmesh[PlayerBulletMod_Max];

// ���b�V�������_���[
static CStaticMeshRenderer g_staticmeshrenderer[PlayerBulletMod_Max];

//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[//

//���b�V�������_��
static CMeshRenderer g_meshrenderer[PlayerBulletMod_Max];

//�}�e���A��
static CMaterial g_material;

// �{�b�N�X���b�V��
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb[PlayerBulletMod_Max];

//�@�v���C���[�v���C���[�o���b�g�ʒu�擾
DirectX::XMFLOAT3 GetPositionPlayerBullet(int idx) {
	return g_playerbullet[idx].position;
}

//�@�v���C���[�o���b�g�}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxPlayerBullet(int idx) {

	Matrix bmtx;
	DX11MakeWorldMatrixRadian(
		bmtx,
		g_playerbullet[idx].scale,
		g_playerbullet[idx].rotation,
		g_playerbullet[idx].position);

	return bmtx;
}

// OBB�擾
BoundingBoxOBB GetPlayerBulletOBB(int idx) {
	Matrix mtx = GetMtxPlayerBullet(idx);
	return g_obb[g_playerbullet[idx].BulletType].MakeOBB(mtx, g_playerbullet[idx].scale);
}

PlayerBullet InitializPlayerBullet() {
	PlayerBullet obj;
	obj.sts = -1;
	obj.BulletType = 0;
	obj.position = { 0,0,0 };
	obj.rotation = { 0,0,0 };
	obj.scale = { 11.0f,11.0f,20.0f };
	obj.SpeedBullet = 0.4f;
	obj.Dame = 20;

	return obj;
}

PlayerBullet InitializPlayerBullet_Sura() {

	PlayerBullet obj;
	obj.sts = -1;
	obj.BulletType = 1;
	obj.position = { 0,0,0 };
	obj.rotation = { 0,0,0 };
	obj.scale = { 10.0f,10.0f,12.0f };
	obj.SpeedBullet = 0.2f;
	obj.Dame = ENEMYBULLETDAME_Sura;

	return obj;
}

PlayerBullet InitializPlayerBullet_SuraLeaf() {

	PlayerBullet obj;
	obj.sts = -1;
	obj.BulletType = 2;
	obj.position = { 0,0,0 };
	obj.rotation = { 0,0,0 };
	obj.scale = { 9.0f,11.0f,10.0f };
	obj.SpeedBullet = 0.2f;
	obj.Dame = ENEMYBULLETDAME_SuraLeaf;

	return obj;
}

//�@�v���C���[�o���b�g��������
void InitPlayerBullet() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// ���_�V�F�[�_
		"shader/vertexLightingPS.hlsl");				// �s�N�Z���V�F�[�_

	// ���f���t�@�C����
	std::string filename[] = {
		u8"assets\\model\\nekobullet.obj",
		u8"assets\\model\\suraBullet.obj",
		u8"assets\\model\\leafbullet.obj",
	};
	

	for (int i = 0; i < PlayerBulletMod_Max; i++) {
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

	for (int i = 0; i < PlayerBullet_Max; i++) {
		static int cnt = 0;
		switch (cnt)
		{
		case 0://�A�r���e�B�o���b�g������
			g_playerbullet[i] = InitializPlayerBullet();
			cnt++;
			break;
		case 1://Sura�o���b�g������
			g_playerbullet[i] = InitializPlayerBullet_Sura();
			cnt++;
			break;
		case 2://SuraGreen�o���b�g������
			g_playerbullet[i] = InitializPlayerBullet_SuraLeaf();
			cnt = 0;
			break;
		}
	}

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[//
	
	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 0.5f);	//���E���J���[
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.TextureEnable = FALSE;

	g_material.Init(mtrl);
}

//�@�v���C���[�o���b�g�X�V����
void UpdatePlayerBullet() {
	
	MovePlayerBullet();

}

//�@�v���C���[�o���b�g�`�揈��
void DrawPlayerBullet() {

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	for (int i = 0; i < PlayerBullet_Max; i++) {
		if (g_playerbullet[i].sts == 1) {
			// ���[���h�ϊ��s�񐶐�
			Matrix mtx;
			DX11MakeWorldMatrixRadian(
				mtx,
				g_playerbullet[i].scale,							// �X�P�[��
				g_playerbullet[i].rotation,							// �p��
				g_playerbullet[i].position);						// �ʒu

			// GPU�ɍs����Z�b�g����
			Renderer::SetWorldMatrix(&mtx);

			// �V�F�[�_�[��GPU�ɃZ�b�g
			g_shader.SetGPU();
			// ���f���`��
			g_staticmeshrenderer[g_playerbullet[i].BulletType].Draw();

			////OBB��񐶐�
			////���E�{�b�N�X�`��
			//mtx = g_obb[g_playerbullet[i].BulletType].MakeWorldMtx(g_playerbullet[i].scale, mtx);
			//Renderer::SetWorldMatrix(&mtx);

			//// ���E�{�b�N�X�`��
			//g_material.SetGPU();
			//g_meshrenderer[g_playerbullet[i].BulletType].Draw();
		}
	}

}

//�@�v���C���[�o���b�g�I������
void ExitPlayerBullet() {

	for (int i = 0; i < PlayerBulletMod_Max; i++) {
		g_staticmeshrenderer[i].Uninit();
	}

}

//�e�����Ă�������`�F�b�N
bool CheckFirePlayerBullet(int idx,int Type) {
	
	bool nur = false;
	//���˂���e���f�����I�΂�Ă��邩
	if (g_playerbullet[idx].BulletType == Type) {
		//���e�������Ă邩
		if (GetCatchBullet(Type) > 0) {
			nur = true;
		}
	}
	
	return nur;
}

//�@�o���b�g���ˏ���
void FirePlayerBullet(int Type) {
	
	for (int i = 0; i < PlayerBullet_Max; i++) {
		if (CheckFirePlayerBullet(i,Type)) {
			//�g�p�ς݃o���b�g���
			if (g_playerbullet[i].sts == 1) {

			}

			//���g�p�o���b�g�擾->�g�p
			if (g_playerbullet[i].sts == -1) {

				//�o���b�g���v���C���[�̍��W���擾
				g_playerbullet[i].position = GetPositionPlayer();
				//�v���C���[��I�Ԗڂ̔��˕����擾
				g_playerbullet[i].rotation = GetRotationPlayer();

				Matrix pbmtx = GetMtxPlayer();

				g_playerbullet[i].vector.x = pbmtx._31;
				g_playerbullet[i].vector.y = pbmtx._32;
				g_playerbullet[i].vector.z = pbmtx._33;

				MakeBulletFireParticle(g_playerbullet[i].position, Color(0.8f, 0.0f, 0.8f, 1.0f));

				g_playerbullet[i].sts = 1;
				PlaySound(FireSE);
				CatchCntDown(Type);
				break;//���g�p���������for����o��
			}
		}
	}

}

//�@�v���C���[�o���b�g�ړ�
void MovePlayerBullet() {
	
	for (int i = 0; i < PlayerBullet_Max; i++) {
		if (g_playerbullet[i].sts == 1) {
			g_playerbullet[i].position.x -= g_playerbullet[i].vector.x * g_playerbullet[i].SpeedBullet;
			g_playerbullet[i].position.z -= g_playerbullet[i].vector.z * g_playerbullet[i].SpeedBullet;

		}
	
		if (MapOutCheck(g_playerbullet[i].position)) {
			g_playerbullet[i].sts = -1;

		}
	}

}

//�@�v���C���[�o���b�g�_���[�W�擾
int GetPlayerBulletDame(int idx) {
	return	g_playerbullet[idx].Dame;
}

//�@�v���C���[�o���b�g�ƃG�l�~�[�̓����蔻��
bool HitCheckEnemy(int idx) {

	static	int BulletCnt = 0;
	for (int i = 0; i < PlayerBullet_Max; i++) {
		if (g_playerbullet[i].sts == 1) {
			bool sts = HitCheckOBB(GetEnemyOBB(idx), GetPlayerBulletOBB(i));
			if (sts) {
				g_playerbullet[i].sts = -1;
				BulletCnt = i;
				EnemyHPReduce(idx, g_playerbullet[i].Dame);				
				ScoreUp(1);
				SetHitStop(8);
				//�G�l�~�[���S���o�I��
				EnemyDeat(idx, i);
				return true;
			}
		}
	}

	EnemyKnockBack(BulletCnt,idx);

	return false;
}

//�@�v���C���[�o���b�g�ƃ{�X�̓����蔻��
bool HitCheckBoss() {

	static	int knockCnt = 0;
	for (int i = 0; i < PlayerBullet_Max; i++) {
		if (g_playerbullet[i].sts == 1) {
			bool sts = HitCheckOBB(GetBossOBB(), GetPlayerBulletOBB(i));
			if (sts) {
				g_playerbullet[i].sts = -1;
				knockCnt = i;
				BossHPReduce(g_playerbullet[i].Dame);
				ScoreUp(1);
				BossDeat(i);

				return true;
			}
		}

	}

	BossKnockBack(knockCnt);

	return false;
}

