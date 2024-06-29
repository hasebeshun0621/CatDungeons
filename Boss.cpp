#include"Boss.h"

using namespace DirectX::SimpleMath;

//�@�{�X�\����
Boss g_boss;

// �V�F�[�_�[
static CShader		g_shader;

// �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
static CStaticMesh g_staticmesh;

// ���b�V�������_���[
static CStaticMeshRenderer g_staticmeshrenderer;

static Vector3	g_move = { 0.0f,0.0f,0.0f };			// �ړ���

//���b�V�������_��
static CMeshRenderer g_meshrenderer;

//�}�e���A��
static CMaterial g_material;

// �{�b�N�X���b�V��
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb;

//�@�{�X�|�W�V�����擾
DirectX::XMFLOAT3 GetBossPosition() {
	return g_boss.position;
}
//�@�{�X��]�擾
DirectX::XMFLOAT3 GetBossRotation() {
	return g_boss.rotation;
}
//�@�{�X��]�ڕW�p�x�擾
DirectX::XMFLOAT3 GetBossDestrot() {
	return g_boss.destrot;
}

//�@�{�X�}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxBoss() {

	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_boss.scale,
		g_boss.rotation,
		g_boss.position);

	return mtx;
}

// �{�XOBB�擾
BoundingBoxOBB GetBossOBB() {
	Matrix mtx = GetMtxBoss();
	return g_obb.MakeOBB(mtx, g_boss.scale);
}

//�@�{�X�X�e�[�^�X�擾
int GetStsBoss() {
	return g_boss.sts;
}

//�@�{�X������
Boss InitializBoss() {

	Boss obj;
	obj.sts = 1;
	obj.Type = 0;
	obj.position.x = (rand() % (int)FieldX) - FieldX / 2;
	obj.position.y = 0;
	obj.position.z = (rand() % (int)FieldZ) - FieldZ / 2;
	obj.rotation = DirectX::XMFLOAT3{ 0,0,0 };
	obj.scale = DirectX::XMFLOAT3{ 5.0f,5.0f,5.0f };
	obj.RATE_ROTATE_ENEMY = 0.05f;
	obj.MoveSpeed = 0.01f;
	obj.HP = ENEMYHP_BOSS;
	obj.KeyGet = false;
	obj.ItemGet = false;
	obj.KnockSwitch = false;
	obj.Knockfcnt = 2;
	obj.DeathSwitch = false;
	obj.pbDeat;
	obj.fcnt = 0;
	obj.fullcnt = rand() % 10 + 40;

	return obj;
}

//�@�{�X��������
void InitBoss() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// ���_�V�F�[�_
		"shader/vertexLightingPS.hlsl");				// �s�N�Z���V�F�[�_

	// ���f���t�@�C����
	std::string filename[] = {
		u8"assets\\model\\boss.obj",
	};

	// ���b�V�������i�W�I���g���f�[�^�j
	g_staticmesh.Init(filename[0]);

	// �`��ׂ̈̃f�[�^����
	g_staticmeshrenderer.Init(g_staticmesh);

	// OBB����
	g_obb.Caliculate(g_staticmesh.GetVertices());

	// OBB�`��p���b�V������
	Vector3 diff = g_obb.GetMax() - g_obb.GetMin();
	g_box.Init(diff.x, diff.y, diff.z, Color(1, 1, 1, 1));
	g_meshrenderer.Init(g_box);

	g_boss = InitializBoss();

	//�G�l�~�[�̒��̈�̂Ɍ���������
	g_boss.KeyGet = true;

	//50���̊m���ň�т̃G�l�~�[�ɃA�C�e����������
	g_boss.ItemGet = true;

	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 0.3f);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.TextureEnable = FALSE;

	g_material.Init(mtrl);

}

//�@�{�X�X�V����
void UpdateBoss() {

	BossLookPlayer();
	BossFall();
	BossDeathAnime();
	BossMove();

	if (GetStsBoss() == 1) {
		g_boss.fcnt++;
		if (g_boss.fcnt >= g_boss.fullcnt) {
			g_boss.fcnt = 0;
			FireBossBullet(GetBossPosition(), GetBossDestrot());
		}

		bool sts = HitCheckBoss();
		if (sts) {
			if (g_boss.HP <= 0) {
				SetHitStop(25);
			}
			g_boss.KnockSwitch = true;
		}
	}

}

//�@�{�X�`�揈��
void DrawBoss() {

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();
	
	if (g_boss.sts == 1) {
			// ���[���h�ϊ��s�񐶐�
			Matrix mtx;
			DX11MakeWorldMatrixRadian(
				mtx,
				g_boss.scale,							// �X�P�[��
				g_boss.rotation,						// �p��
				g_boss.position);						// �ʒu

			// GPU�ɍs����Z�b�g����
			Renderer::SetWorldMatrix(&mtx);

			// �V�F�[�_�[��GPU�ɃZ�b�g
			g_shader.SetGPU();

			// ���f���`��
			g_staticmeshrenderer.Draw();
			//-------------------------------------//box�`��
			////���E�{�b�N�X�`��
			//mtx = g_obb.MakeWorldMtx(g_boss.scale, mtx);
			//Renderer::SetWorldMatrix(&mtx);
			//g_material.SetGPU();
			//g_meshrenderer.Draw();
	}

}

//�@�{�X�I������
void ExitBoss() {
		g_staticmeshrenderer.Uninit();
}

//�@�{�X���v���C���[�Ɍ�����
void BossLookPlayer() {

	//�v���C���[�|�W�V�����𒲂ׂ�
	DirectX::SimpleMath::Vector3 playerpos = GetPositionPlayer();

	//atan2�i�A�[�N�^���W�F���g2�j���g����]�ڕW�p�x���v�Z
	g_boss.destrot.y = atan2f(-(playerpos.x - g_boss.position.x), -(playerpos.z - g_boss.position.z));

	//���݂̌����ƍ������v�Z����
	float fDiffRotY = g_boss.destrot.y - g_boss.rotation.y;

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
	g_boss.rotation.y += fDiffRotY * g_boss.RATE_ROTATE_ENEMY;
	if (g_boss.rotation.y > DirectX::XM_PI) {
		g_boss.rotation.y -= DirectX::XM_PI * 2.0f;
	}

	if (g_boss.rotation.y < -DirectX::XM_PI) {
		g_boss.rotation.y += DirectX::XM_PI * 2.0f;
	}

}

//�@�{�X��HP�����炷
void BossHPReduce(int dame) {
	g_boss.HP -= dame;
}

//�{�X�m�b�N�o�b�N
void BossKnockBack(int idx) {
	
	//�q�b�g�����e�̐i�s�������擾
	Matrix PB = GetMtxPlayerBullet(idx);
	int dame = GetPlayerBulletDame(idx);

	if (g_boss.KnockSwitch) {
		if (g_boss.Knockfcnt < 10) {
			g_boss.position.x -= PB._31 * (dame / 10) / g_boss.Knockfcnt;
			g_boss.position.z -= PB._33 * (dame / 10) / g_boss.Knockfcnt;
			g_boss.Knockfcnt++;
		}
		else {
			g_boss.KnockSwitch = false;
			g_boss.Knockfcnt = 2;

		}
	}

}

//�{�X����
void BossFall() {
	
	static float G = 0.05f;//�d��
	static float fcnt = 0;//���Ԃ�\��

	if (g_boss.sts == 1) {
		//�����𔻒肷��֐�������
		bool sts = FieldOutCheck(g_boss.position);

		if (sts) {
			float jcnt = fcnt * G;//�d�͉����x
			//�������x
			g_boss.position.y -= jcnt;
			fcnt++;
		}
	
		//�{�X�̏����鍂��
		if (g_boss.position.y <= -200.0f) {
			g_boss.sts = -1;
			fcnt = 0;
		}
	}

}

//�@�{�X�ړ�
void BossMove() {

	static	float radian;
	static	int direction;
	static float movefcnt;
	static bool null=true;
	//�G�l�~�[�̏�O���`�F�b�N
	static DirectX::XMFLOAT3 outcheck = {0.0f,0.0f,0.0f};

	if (null) {
		srand(time(NULL));//�����_���V�[�h�ύX
		//�{�X����������8����
		direction = rand() % 8;
		//�{�X�̈ړ���
		movefcnt = rand() % 10 + 40.0f;
		null = false;
	}

	if (movefcnt > 0&&!null) {
		movefcnt--;
		switch (direction)
		{
		case 0://��
			radian = DirectX::XM_PI * 0.0f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;
			break;
		case 1://�O
			radian = DirectX::XM_PI * 1.0f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;
			break;
		case 2://�E
			radian = DirectX::XM_PI * 0.5f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;
			break;
		case 3://��
			radian = -DirectX::XM_PI * 0.5f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;
			break;
		case 4://�E���
			radian = DirectX::XM_PI * 0.25f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;
			break;
		case 5://�E�O
			radian = DirectX::XM_PI * 0.75f;
			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;

			break;
		case 6://����
			radian = -DirectX::XM_PI * 0.25f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;
			break;
		case 7://���O
			radian = -DirectX::XM_PI * 0.75f;

			g_move.x -= sinf(radian) * g_boss.MoveSpeed;
			g_move.z -= cosf(radian) * g_boss.MoveSpeed;

			break;
		}
		outcheck = g_boss.position+g_move;
		//��O���`�F�b�N
		if (FieldOutCheck(outcheck)) {
			null = true;
			g_move = DirectX::XMFLOAT3{0.0f,0.0f,0.0f};
		}
		else {
			g_boss.position.x += g_move.x;
			g_boss.position.z += g_move.z;
		}
	}
	else {
		null = true;
	}

}

//�@�{�X���S�`�F�b�N
void BossDeat(int bidx) {

	if (g_boss.HP <= 0) {
		g_boss.DeathSwitch = true;
		//�q�b�g�����e�̐i�s�������擾
		g_boss.pbDeat = GetMtxPlayerBullet(bidx);
		MakeKillParticle(GetPositionPlayerBullet(bidx));
		PlaySound(KOSE);
	}
	else {
		MakeHitParticle(GetPositionPlayerBullet(bidx));
		PlaySound(HitSE);
	}

}

//�@�{�X���S���o
void BossDeathAnime() {

	if (g_boss.DeathSwitch) {
		Matrix pb = g_boss.pbDeat;
		g_boss.position.x -= pb._31 / 3;
		g_boss.position.y += 2.0f;
		g_boss.position.z -= pb._33 / 3;
		g_boss.rotation.y += 0.4f;
	}

}



