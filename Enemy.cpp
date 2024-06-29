#include"Enemy.h"

using namespace DirectX::SimpleMath;

//�@�G�l�~�[�\����
Enemy g_enemy[Enemy_Max];

// �V�F�[�_�[
static CShader		g_shader;

// �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
static CStaticMesh g_staticmesh[EnemyMod_Max];

// ���b�V�������_���[
static CStaticMeshRenderer g_staticmeshrenderer[EnemyMod_Max];

static Vector3	g_move = { 0.0f,0.0f,0.0f };			// �ړ���

//�E�F�[�u�ɏo��G�l�~�[�̍ő吔
int g_WaveEnemyMax = 0;

//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[//

//���b�V�������_��
static CMeshRenderer g_meshrenderer[EnemyMod_Max];

//�}�e���A��
static CMaterial g_material;

// �{�b�N�X���b�V��
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb;

//�@�G�l�~�[��idx�Ԗڂ̈ʒu�擾
DirectX::XMFLOAT3 GetEnemyPosition(int idx) {
	return g_enemy[idx].position;
}
//�@�G�l�~�[��idx�Ԗڂ̉�]�擾
DirectX::XMFLOAT3 GetEnemyRotation(int idx) {
	return g_enemy[idx].rotation;
}
//�@�G�l�~�[��idx�Ԗڂ̉�]�ڕW���擾
DirectX::XMFLOAT3 GetEnemyDestrot(int idx) {
	return g_enemy[idx].destrot;
}
//�@�G�l�~�[��idx�Ԗڂ̃}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxEnemy(int idx) {

	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_enemy[idx].scale,
		g_enemy[idx].rotation,
		g_enemy[idx].position);

	return mtx;
}

//�@�G�l�~�[��idx�Ԗڂ�OBB�擾
BoundingBoxOBB GetEnemyOBB(int idx) {
	Matrix mtx = GetMtxEnemy(idx);
	return g_obb.MakeOBB(mtx, g_enemy[idx].scale);
}

//�@�G�l�~�[�X�e�[�^�X���擾
int GetStsEnemy(int idx) {
	return g_enemy[idx].sts;
}
//�@�G�l�~�[�^�C�v�擾
int GetEnemyType(int idx) {
	return g_enemy[idx].EnemyType;
}
//�@���������Ă��邩�`�F�b�N
bool GetKeyEnemy(int idx) {
	return g_enemy[idx].KeyGet;
}
//�@�A�C�e���������Ă��邩�`�F�b�N
bool GetItemEnemy(int idx) {
	return g_enemy[idx].ItemGet;
}

//�@�X���C��������
Enemy InitializSura() {
	
	Enemy obj;
	obj.sts = 1;
	obj.EnemyType = 0;
	obj.position.x = (rand() % (int)FieldX) - FieldX / 2;
	obj.position.y = 0;
	obj.position.z = (rand() % (int)FieldZ) - FieldZ / 2;
	obj.rotation = DirectX::XMFLOAT3{ 0,0,0 };
	obj.scale = DirectX::XMFLOAT3{ 10.0f,10.0f,10.0f };
	obj.RATE_ROTATE_ENEMY = 0.05f;
	obj.HP = ENEMYHP_Sura;
	obj.KeyGet = false;
	obj.ItemGet = false;
	obj.KnockSwitch = false;
	obj.DeathSwitch = false;
	obj.pbDeat;
	obj.Knockfcnt = 2;
	obj.fcnt = 0;
	obj.fullcnt = rand() % 50 + 100;

	return obj;
}

//�@�΃X���C��������
Enemy InitializSuraGreen() {

	Enemy obj;
	obj.sts = 1;
	obj.EnemyType = 1;
	obj.position.x = (rand() % (int)FieldX) - FieldX / 2;
	obj.position.y = 0;
	obj.position.z = (rand() % (int)FieldZ) - FieldZ / 2;
	obj.rotation = DirectX::XMFLOAT3{ 0,0,0 };
	obj.scale = DirectX::XMFLOAT3{ 10.0f,10.0f,10.0f };
	obj.RATE_ROTATE_ENEMY = 0.05f;
	obj.HP = ENEMYHP_SuraGreen;
	obj.KeyGet = false;
	obj.ItemGet = false;
	obj.KnockSwitch = false;
	obj.DeathSwitch = false;
	obj.pbDeat;
	obj.Knockfcnt = 2;
	obj.fcnt = 0;
	obj.fullcnt = rand() % 50 + 100;	//�e���ˊԊu

	return obj;
}

//�@�G�l�~�[��������
void InitEnemy(int idx) {
	enum Wave
	{
		Tutorial = 0,
		Wave1,
		Wave2,
		Wave3,
		Boss,
		Treasure ,
	};

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// ���_�V�F�[�_
		"shader/vertexLightingPS.hlsl");				// �s�N�Z���V�F�[�_

	// ���f���t�@�C����
	std::string filename[] = {
		u8"assets\\model\\sura.obj",
		u8"assets\\model\\suragreen.obj",
	};

	for (int i = 0; i < EnemyMod_Max; i++) {
		// ���b�V�������i�W�I���g���f�[�^�j
		g_staticmesh[i].Init(filename[i]);

		// �`��ׂ̈̃f�[�^����
		g_staticmeshrenderer[i].Init(g_staticmesh[i]);

		// OBB����
		g_obb.Caliculate(g_staticmesh[i].GetVertices());

		// OBB�`��p���b�V������
		Vector3 diff = g_obb.GetMax() - g_obb.GetMin();
		g_box.Init(diff.x, diff.y, diff.z, Color(1, 1, 1, 1));
		g_meshrenderer[i].Init(g_box);
	}

	//�E�F�[�u�ɓo�ꂳ����G�l�~�[�̐�
	int sura = 0;
	int suraG = 0;
	//���ɏ�������z��ԍ�
	int pointa = 0;
	switch (idx)
	{
	case Tutorial:
		break;
	case Wave1:
		//�E�F�[�u�ɓo�ꂳ����G�l�~�[�̐���ݒ�
		sura = 3;
		suraG = 0;
		break;
	case Wave2:
		sura = 2;
		suraG = 2;
		break;
	case Wave3:
		sura = 2;
		suraG = 3;
		break;
	case Boss:
		sura = 1;
		suraG = 1;
		break;

	}
	
	for (int i = 0; i < sura; i++) {
		g_enemy[pointa] = InitializSura();
		pointa++;
	}
	for (int i = 0; i < suraG; i++) {
		g_enemy[pointa] = InitializSuraGreen();
		pointa++;
	}

	g_WaveEnemyMax = pointa;

	//�[��������̂���
	if (g_WaveEnemyMax > 0) {
		if (GetScene() != BOSS) {
			//�G�l�~�[�̒��̈�̂Ɍ���������
			g_enemy[rand() % g_WaveEnemyMax].KeyGet = true;

			for (int i = 0; i < 2; i++) {
				//50���̊m���ň�т̃G�l�~�[�ɃA�C�e����������
				int num = rand() % (g_WaveEnemyMax * 2);
				if (num < g_WaveEnemyMax && GetKeyEnemy(num) == false) {
					g_enemy[num].ItemGet = true;
				}
			}
		}
	}

	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 0.3f);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.TextureEnable = FALSE;

	g_material.Init(mtrl);

}

//�@�G�l�~�[�X�V����
void UpdateEnemy() {

	EnemyLookPlayer();
	EnemyFall();
	EnemyDeathAnime();

	for (int i = 0; i < g_WaveEnemyMax; i++) {
		if (GetStsEnemy(i) == 1&&!g_enemy[i].DeathSwitch) {
			g_enemy[i].fcnt++;
			if (g_enemy[i].fcnt >= g_enemy[i].fullcnt) {
				g_enemy[i].fcnt = 0;
				FireEnemyBullet(GetEnemyPosition(i), GetEnemyDestrot(i),g_enemy[i].EnemyType);
			}
		}
	}

	for (int i = 0; i < g_WaveEnemyMax; i++) {
		if (GetStsEnemy(i) == 1) {
			bool sts = HitCheckEnemy(i);
			if (sts) {
				if (g_enemy[i].HP <= 0) {
					KeyDrop(i);
					HPPortionDrop(i);
				}
				g_enemy[i].KnockSwitch = true;
			}
		}
	}

}

//�@�G�l�~�[�`�揈��
void DrawEnemy() {
	
	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	for (int i = 0; i < g_WaveEnemyMax; i++) {
		if (g_enemy[i].sts == 1) {
			// ���[���h�ϊ��s�񐶐�
			Matrix mtx;
			DX11MakeWorldMatrixRadian(
				mtx,
				g_enemy[i].scale,							// �X�P�[��
				g_enemy[i].rotation,						// �p��
				g_enemy[i].position);						// �ʒu

			// GPU�ɍs����Z�b�g����
			Renderer::SetWorldMatrix(&mtx);

			// �V�F�[�_�[��GPU�ɃZ�b�g
			g_shader.SetGPU();

			// ���f���`��
			g_staticmeshrenderer[g_enemy[i].EnemyType].Draw();

			//-------------------------------------//box�`��
			////���E�{�b�N�X�`��
			//mtx = g_obb.MakeWorldMtx(g_enemy[i].scale, mtx);
			//Renderer::SetWorldMatrix(&mtx);

			//g_material.SetGPU();
			//g_meshrenderer[g_enemy[i].EnemyType].Draw();
		}
	}

}

//�@�G�l�~�[�I������
void ExitEnemy() {

	for (int i = 0; i < EnemyBulletMod_Max; i++) {
		g_staticmeshrenderer[i].Uninit();
	}

}

//�@�G�l�~�[���v���C���[�Ɍ�����
void EnemyLookPlayer() {

	for (int i = 0; i < g_WaveEnemyMax; i++) {
		if (!g_enemy[i].DeathSwitch) {
			//�v���C���[�|�W�V�����𒲂ׂ�
			DirectX::SimpleMath::Vector3 playerpos = GetPositionPlayer();

			//atan2�i�A�[�N�^���W�F���g2�j���g����]�ڕW�p�x���v�Z
			g_enemy[i].destrot.y = atan2f(-(playerpos.x - g_enemy[i].position.x), -(playerpos.z - g_enemy[i].position.z));

			//���݂̌����ƍ������v�Z����

			float fDiffRotY = g_enemy[i].destrot.y - g_enemy[i].rotation.y;

			// �␳�i�|�P�W�O�`�P�W�O�͈̔́j
			if (fDiffRotY > DirectX::XM_PI)
			{
				fDiffRotY -= DirectX::XM_PI * 2.0f;
			}

			if (fDiffRotY < -DirectX::XM_PI)
			{
				fDiffRotY += DirectX::XM_PI * 2.0f;
			}

			// ��]�p�x�v�Z
			g_enemy[i].rotation.y += fDiffRotY * g_enemy[i].RATE_ROTATE_ENEMY;
			if (g_enemy[i].rotation.y > DirectX::XM_PI)
			{
				g_enemy[i].rotation.y -= DirectX::XM_PI * 2.0f;
			}

			if (g_enemy[i].rotation.y < -DirectX::XM_PI)
			{
				g_enemy[i].rotation.y += DirectX::XM_PI * 2.0f;
			}
		}
	}

}

//�@�G�l�~�[�^�C�v�̂g�o�����炷
void EnemyHPReduce(int idx,int dame) {

	g_enemy[idx].HP -= dame;

}

//�@�G�l�~�[�m�b�N�o�b�N
void EnemyKnockBack(int bulletcnt, int idx) {

	//�q�b�g�����e�̐i�s�������擾
	Matrix PB = GetMtxPlayerBullet(bulletcnt);
	int dame = GetPlayerBulletDame(bulletcnt);

	if (g_enemy[idx].KnockSwitch) {
		if (g_enemy[idx].Knockfcnt < 10) {
			g_enemy[idx].position.x -= PB._31 / g_enemy[idx].Knockfcnt * (float)(dame / 5);
			g_enemy[idx].position.z -= PB._33 / g_enemy[idx].Knockfcnt * (float)(dame / 5);
			//�����p�[�e�B�N�����o��
			MakeDashParticle(g_enemy[idx].position);
			g_enemy[idx].Knockfcnt++;
		}
		else {
			g_enemy[idx].KnockSwitch = false;
			g_enemy[idx].Knockfcnt = 2;
		}
	}

}

//�@�G�l�~�[����
void EnemyFall() {

	static float G = 0.05f;//�d��
	static float fcnt = 0;//���Ԃ�\��

	for (int i = 0; i < Enemy_Max; i++) {
		if (g_enemy[i].sts == 1) {
			//�����𔻒肷��
			bool sts = FieldOutCheck(g_enemy[i].position);

			if (sts) {
				float jcnt = fcnt * G;//�d�͉����x
				//�������x
				g_enemy[i].position.y -= jcnt;
				fcnt++;
			}

			if (g_enemy[i].position.y <= -FallY) {
				g_enemy[i].sts = -1;
				FallKeyDrop(i);
				FallHPPotionDrop(i);
				fcnt = 0;
			}
		}
	}

}

//�@�G�l�~�[���S�`�F�b�N
void EnemyDeat(int idx,int bidx) {

	if (g_enemy[idx].HP <= 0) {
		g_enemy[idx].DeathSwitch = true;
		//�q�b�g�����e�̐i�s�������擾
		g_enemy[idx].pbDeat = GetMtxPlayerBullet(bidx);
		MakeKillParticle(GetPositionPlayerBullet(bidx));
		PlaySound(KOSE);
	}
	else {
		MakeHitParticle(GetPositionPlayerBullet(bidx));
		PlaySound(HitSE);
	}

}

//�@�G�l�~�[���S���o
void EnemyDeathAnime() {

	for (int i = 0; i < Enemy_Max; i++) {
		if (g_enemy[i].DeathSwitch) {
			Matrix pb = g_enemy[i].pbDeat;
			g_enemy[i].position.x -= pb._31/3;
			g_enemy[i].position.y += 1.5f;
			g_enemy[i].position.z -= pb._33/3;
			g_enemy[i].rotation.x+=0.4f;
		}
	}

}


