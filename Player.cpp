#include	"Player.h"

using namespace DirectX::SimpleMath;

//�v���C���[�\����
Player g_player;

// �V�F�[�_�[
static CShader		g_shader;

// �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
static CStaticMesh g_staticmesh;

// ���b�V�������_���[
static CStaticMeshRenderer g_staticmeshrenderer;

static Vector3	g_move = { 0.0f,0.0f,0.0f };			// �ړ���
static Vector3	g_destrot = { 0.0f,0.0f,0.0f };			// �ڕW��]�p�x

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

// �ʒu�擾
DirectX::XMFLOAT3 GetPositionPlayer() {
	return g_player.position;
}

//�@�p���擾
DirectX::XMFLOAT3 GetRotationPlayer() {
	return g_player.rotation;
}

//�@�T�C�Y�擾
DirectX::XMFLOAT3 GetScalePlayer() {
	return g_player.scale;
}

//�@�v���C���[�}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxPlayer() {
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_player.scale,
		g_player.rotation,
		g_player.position);
	return mtx;
}

// OBB�擾
BoundingBoxOBB GetPlayerOBB() {

	Matrix mtx = GetMtxPlayer();
	return g_obb.MakeOBB(mtx, g_player.scale);
}

//�@�ړ��ʎ擾
DirectX::XMFLOAT3 GetMovePlayer() {
	return g_move;
}
//�@��]�ڕW�p�x�擾
DirectX::XMFLOAT3 GetDestrotPlayer() {
	return g_destrot;
}
//�v���C���[�����蔻��擾
bool GetPlayerHitSts() {
	return g_player.HitSts;
}
//�@HP�|�[�V����������
int GetHPPortionCnt() {
	return g_player.HPPortionCnt;
}
//�@HP�|�[�V�����𑝂₷
void HPPortionCntUp() {
	g_player.HPPortionCnt++;
}
//�@HP�|�[�V���������炷
void HPPortionCntDown() {
	g_player.HPPortionCnt--;
}

//�@���I��ł���A�C�e����
int GetItemPointer() {
	return	g_player.ItemPointer;
}
//�@���I��ł���o���b�g
int GetSelectBullet() {
	return	g_player.ItemSideCnt;
}

//�@���̃v���C���[��HP���Q�b�g
int GetPlayerHp() {
	return g_player.HP;
}

//�@�v���C���[��HP�𑝂₷
void PlayerHPUP(int num) {

	g_player.HP += num;
	if (PlayerHP_Max <= g_player.HP) {
		g_player.HP = PlayerHP_Max;
	}

}

//�@�v���C���[��HP�����炷
void PlayerHPDOWN(int num) {

	g_player.HP -= num;
	PlaySound(DameSE);

}

//�@�v���C���[�X�^�~�i�����炷
void PlayerSTDOWN(int num) {
	g_player.ST -= num;
}

//�@���̃v���C���[��ST���Q�b�g
int GetPlayerSt() {
	return g_player.ST;
}

//�@�v���C���[������
Player initializePlayer() {

	Player obj;
	// �v���C���̏��(S,R,T)
	obj.position = { 0.0f,0.0f,0.0f };
	obj.rotation = { 0.0f,0.0f,0.0f };
	obj.scale = { 15.0f,15.0f,15.0f };
	obj.VALUE_MOVE_MODEL = 0.8f;						
	obj.VALUE_ROTATE_MODEL = DirectX::XM_PI * 0.1f;		
	obj.RATE_ROTATE_MODEL = 0.15f;						
	obj.RATE_MOVE_MODEL = 0.3f;							
	obj.HitSts = true;
	obj.EvasionSwitch = false;
	obj.ItemPointer = 0;
	obj.ItemSideCnt = 1;
	obj.HPPortionCnt = 0;
	obj.HP = PlayerHP_Max;
	obj.ST = 200;
	obj.STcnt = 0;

	return obj;

}

// �v���C���[��������
void InitPlayer() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// ���_�V�F�[�_
		"shader/vertexLightingPS.hlsl");				// �s�N�Z���V�F�[�_

	// ���f���t�@�C����
	std::string filename[] = {
		u8"assets\\model\\neko.obj"
	};

	// ���b�V�������i�W�I���g���f�[�^�j
	g_staticmesh.Init(filename[0]);

	// �`��ׂ̈̃f�[�^����
	g_staticmeshrenderer.Init(g_staticmesh);

//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[//
	
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

	g_player = initializePlayer();

	g_material.Init(mtrl);

}

// �v���C���[�X�V����
void UpdatePlayer() {
	
	//�v���C���[����
	if (!PlayerFall()) {
		//�v���C���[����
		PlayerControl();
		//�v���C���[���
		PlayerEvasion();
	}


	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LEFT)) {
		if (g_player.ItemPointer < 2) {
			g_player.ItemPointer++;
		}
		else {
			g_player.ItemPointer = 0;
		}
		PlaySound(ItemSelectSE);
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RIGHT)) {
		if (g_player.ItemPointer > 0) {
			g_player.ItemPointer--;
		}
		else {
			g_player.ItemPointer = 2;
		}
		PlaySound(ItemSelectSE);
	}
	
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP)) {
		if (g_player.ItemPointer == 2) {
			g_player.ItemSideCnt--;
			if (g_player.ItemSideCnt <= 0) {
				g_player.ItemSideCnt = PlayerBulletMod_Max-1;
			}
		}
		else {
			g_player.ItemPointer = 2;
		}
		PlaySound(BulletSE);
	}
	
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN)) {
		if (g_player.ItemPointer == 2) {
			g_player.ItemSideCnt++;
			if (g_player.ItemSideCnt >=PlayerBulletMod_Max) {
				g_player.ItemSideCnt = 1;
			}
		}
		else {
			g_player.ItemPointer = 2;
		}
		PlaySound(BulletSE);
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN)) {
		if (g_player.HitSts) {
			//�����e
			if (g_player.ItemPointer == ABILITY) {
				if (g_player.HP - 30 > 0) {
					FirePlayerBullet(0);
					//�e�����ƃ_���[�W�����炤
					PlayerHPDOWN(PlayerAbilityDame);
					MakeBulletFireParticle(g_player.position, Color(1.0f, 0.0f, 0.0f, 1.0f));
				}

			}

			//��
			if (g_player.ItemPointer == HEAL) {
				if (GetHPPortionCnt() > 0) {
					HPPortionCntDown();
					PlayerHPUP(30);
					MakeHealParticle(g_player.position);
					PlaySound(HealSE);
				}
			}

			//�E�����e
			if (g_player.ItemPointer == BULLET) {
				if (GetCatchCnt() > 0) {
					FirePlayerBullet(g_player.ItemSideCnt);

				}
			}
		}
	}
	
	if (NowBagCnt() < BagMax) {
		if (g_player.HitSts) {
			bool sts = HitCheckNet();
			if (sts) {
				ScoreUp(1);
			}
		}
	}

}

// �v���C���[�`�揈��
void DrawPlayer() {

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ���[���h�ϊ��s�񐶐�
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_player.scale,							// �X�P�[��
		g_player.rotation,						// �p��
		g_player.position);						// �ʒu

	// GPU�ɍs����Z�b�g����
	Renderer::SetWorldMatrix(&mtx);

	// �V�F�[�_�[��GPU�ɃZ�b�g
	g_shader.SetGPU();												

	// ���f���`��
	g_staticmeshrenderer.Draw();						
//-------------------------------------//���`��
	//// ���E�{�b�N�X�`��
	//mtx = g_obb.MakeWorldMtx(g_player.scale, mtx);

	//Renderer::SetWorldMatrix(&mtx);
	//g_material.SetGPU();
	//g_meshrenderer.Draw();

}

//�@�v���C���[�I������
void ExitPlayer() {
	g_staticmeshrenderer.Uninit();
}

//�@�v���C���[����܂Ƃ�
void PlayerControl() {

	if (g_player.Control) {
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
			{// ���O�ړ�

				float radian;
				radian = DirectX::XM_PI * 0.75f;

				g_move.x -= sinf(radian) * g_player.VALUE_MOVE_MODEL;
				g_move.z -= cosf(radian) * g_player.VALUE_MOVE_MODEL;

				// �ڕW�p�x���Z�b�g
				g_destrot.y = radian;

			}
			else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
			{// ����ړ�

				float radian;
				radian = DirectX::XM_PI * 0.25f;

				g_move.x -= sinf(radian) * g_player.VALUE_MOVE_MODEL;
				g_move.z -= cosf(radian) * g_player.VALUE_MOVE_MODEL;

				// �ڕW�p�x���Z�b�g
				g_destrot.y = radian;
			}
			else
			{// ���ړ�

				float radian;
				radian = DirectX::XM_PI * 0.50f;

				g_move.x -= sinf(radian) * g_player.VALUE_MOVE_MODEL;
				g_move.z -= cosf(radian) * g_player.VALUE_MOVE_MODEL;

				// �ڕW�p�x���Z�b�g
				g_destrot.y = radian;
			}
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
		{
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)) {
				// �E�O�ړ�

				float radian;
				radian = -DirectX::XM_PI * 0.75f;

				g_move.x -= sinf(radian) * g_player.VALUE_MOVE_MODEL;
				g_move.z -= cosf(radian) * g_player.VALUE_MOVE_MODEL;

				// �ڕW�p�x���Z�b�g
				g_destrot.y = radian;

			}
			else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
			{// �E��ړ�
				float radian;
				radian = -DirectX::XM_PI * 0.25f;

				g_move.x -= sinf(radian) * g_player.VALUE_MOVE_MODEL;
				g_move.z -= cosf(radian) * g_player.VALUE_MOVE_MODEL;

				// �ڕW�p�x���Z�b�g
				g_destrot.y = radian;
			}
			else
			{// �E�ړ�

				float radian;
				radian = -DirectX::XM_PI * 0.50f;

				g_move.x -= sinf(radian) * g_player.VALUE_MOVE_MODEL;
				g_move.z -= cosf(radian) * g_player.VALUE_MOVE_MODEL;

				// �ڕW�p�x���Z�b�g
				g_destrot.y = radian;
			}
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
		{
			// �O�ړ�
			g_move.x -= sinf(DirectX::XM_PI) * g_player.VALUE_MOVE_MODEL;
			g_move.z -= cosf(DirectX::XM_PI) * g_player.VALUE_MOVE_MODEL;

			// �ڕW�p�x���Z�b�g
			g_destrot.y = DirectX::XM_PI * 1.0f;
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
		{
			// ��ړ�
			g_move.x += sinf(DirectX::XM_PI) * g_player.VALUE_MOVE_MODEL;
			g_move.z += cosf(DirectX::XM_PI) * g_player.VALUE_MOVE_MODEL;

			// �ڕW�p�x���Z�b�g
			g_destrot.y = DirectX::XM_PI * 0.0f;
		}

		PlayerDash();

		// �ڕW�p�x�ƌ��݊p�x�Ƃ̍��������߂�
		float diffrot = g_destrot.y - g_player.rotation.y;
		if (diffrot > DirectX::XM_PI) {
			diffrot -= DirectX::XM_PI * 2.0f;
		}

		if (diffrot < -DirectX::XM_PI) {
			diffrot += DirectX::XM_PI * 2.0f;
		}

		// �p�x���x�Ɋ�����t����
		g_player.rotation.y += diffrot * g_player.RATE_ROTATE_MODEL;
		if (g_player.rotation.y > DirectX::XM_PI) {
			g_player.rotation.y -= DirectX::XM_PI * 2.0f;
		}

		if (g_player.rotation.y < -DirectX::XM_PI) {
			g_player.rotation.y += DirectX::XM_PI * 2.0f;
		}

		// �ʒu�ړ�
		g_player.position += g_move;

		// �ړ��ʂɊ�����������(��������)
		g_move += -g_move * g_player.RATE_MOVE_MODEL;
	}

}

//�@�v���C���[�_�b�V��
void PlayerDash() {
	//�_�b�V���{�^��
	//wasd�̂ǂꂩ�������Ă���
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)
		|| CDirectInput::GetInstance().CheckKeyBuffer(DIK_S) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
	{
		//�X�^�~�i���O�ȏ�ŃV�t�g���������Ȃ�_�b�V��
		if ((CDirectInput::GetInstance().CheckKeyBuffer(DIK_RSHIFT) 
			|| CDirectInput::GetInstance().CheckKeyBuffer(DIK_LSHIFT)) 
				&& g_player.ST > 0)
		{

			g_player.VALUE_MOVE_MODEL = 1.2f;
			//�X�^�~�i����
			g_player.ST--;
			g_player.STcnt = 100;//�S�t���[����ɃX�^�~�i����
			//�����G�t�F�N�g���o��
			MakeDashParticle(GetPositionPlayer());
		}
		else
		{
			g_player.VALUE_MOVE_MODEL = 0.8f;
		}
	}
	//�X�^�~�i��
	if (g_player.ST < 200 && g_player.STcnt <= 0) {
		g_player.ST++;
	}
	else {
		g_player.STcnt--;
	}

}

//�v���C���[���
void PlayerEvasion() {

	static float jumpfcnt = 0.0f;
	static Matrix pp;

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RCONTROL) &&
		!g_player.EvasionSwitch && g_player.ST > 30) {

		g_player.EvasionSwitch = true;
		g_player.HitSts = false;
		g_player.Control = false;
		pp = GetMtxPlayer();
		//�X�^�~�i�񕜊Ԋu
		g_player.STcnt = 100;
		PlayerSTDOWN(30);
	}

	if (g_player.EvasionSwitch) {
		//�p�[�e�B�N������
		MakeDashParticle(GetPositionPlayer());

		//�v���C���[�ړ�
		g_player.position.z -= (pp._33 * 0.15f);
		g_player.position.x -= (pp._31 * 0.15f);

		//�v���C���[��]
		g_player.rotation.x -= 0.15f;
		//sign�J�[�u��`��
		jumpfcnt += 0.15f;
		g_player.position.y += sinf(jumpfcnt);
	}

	if (jumpfcnt >= DirectX::XM_PI * 2 || FieldOutCheck(g_player.position)) {
		g_player.EvasionSwitch = false;
		g_player.HitSts = true;
		g_player.Control = true;
		g_player.rotation.x = 0.0f;
		jumpfcnt = 0.0f;
	}

}

//�@�v���C���[����
bool PlayerFall() {

	bool sts = FieldOutCheck(g_player.position);//�����𔻒肷��l������
	static float G = 0.05f;//�d��
	static float fcnt = 0.0f;//���Ԃ�\��
	float jcnt = fcnt * G;//�d�͉����x

	if (sts) {
		//�������x
		g_move.y -= jcnt;
		g_player.position.y += g_move.y;
		fcnt+=0.5f;
		//�������c��]
		g_player.rotation.x -= 0.1f;
	}

	if (g_player.position.y <= -FallY) {
		g_player.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_player.rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_move = DirectX::SimpleMath::Vector3(0,0,0);
		//�����_���[�W
		PlayerHPDOWN(20);
		fcnt = 0.0f;
	}

	return sts;
}

//�@�v���C���[�|�W�V�������Z�b�g
void ResetPosPlayer() {

	g_player.position = initializePlayer().position;
	g_player.rotation = initializePlayer().rotation;
	g_player.EvasionSwitch = false;
	g_player.HitSts = true;
	g_player.Control = true;
	Camera::Init();

}

//�@�v���C���[�m�b�N�o�b�N
bool PlayerKnockBack(int idx,int type) {

	//�m�b�N�o�b�N�J���g
	static int fcnt = 2;
	Matrix EB;
	int dame=0;

	switch (type)
	{
	case 1://�G�l�~�[�e
		//�q�b�g�����e�̐i�s�������擾
		EB = GetMtxEnemyBullet(idx);
		dame = GetEnemyBulletDame(idx);
		break;
	case 2://�{�X�e
		EB = GetMtxBossBullet(idx);
		dame = GetBossBulletDame(idx);
		break;
	}

	if (fcnt < 10) {
		g_player.position.x -= EB._31 / fcnt * (dame / 5);
		g_player.position.z -= EB._33 / fcnt * (dame / 5);
		MakeDashParticle(g_player.position);
		fcnt++;
	}
	else {
		fcnt = 2;
		//�m�b�N�o�b�N�I���
		return false;
	}

	//�m�b�N�o�b�N��
	return true;
}
