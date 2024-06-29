#include "HPPortion.h"

using namespace DirectX::SimpleMath;

//�@HP�|�[�V�����\����
HPPortion g_HPPortion[HPPortion_Max];

// �V�F�[�_�[
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

//�@HP�|�[�V�����}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxHPPortion(int idx) {

	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_HPPortion[idx].scale,
		g_HPPortion[idx].rotation,
		g_HPPortion[idx].position);

	return mtx;
}

//�@HP�|�[�V����OBB�擾
BoundingBoxOBB GetHPPortionOBB(int idx) {
	Matrix mtx = GetMtxHPPortion(idx);
	return g_obb.MakeOBB(mtx, g_HPPortion[idx].scale);
}

//�@HP�|�[�V������������
void InitHPPortion() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// ���_�V�F�[�_
		"shader/vertexLightingPS.hlsl");				// �s�N�Z���V�F�[�_

	// ���f���t�@�C����
	std::string filename[] = {
		u8"assets\\model\\gposhonmini.obj"
	};

	for (int i = 0; i < HPPortion_Max; i++) {
		//������
		g_HPPortion[i].sts = -1;
		g_HPPortion[i].num = 0.5f;
	}

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

//�@HP�|�[�V�����X�V����
void UpdateHPPortion() {

	for (int i = 0; i < HPPortion_Max; i++) {
		//�|�[�V�����A�j���[�V����
		if (g_HPPortion[i].sts == 1) {
			if (g_HPPortion[i].position.y < 0 || g_HPPortion[i].position.y > 20) {
				g_HPPortion[i].num *= -1;
			}

			g_HPPortion[i].position.y += g_HPPortion[i].num;
			g_HPPortion[i].rotation.y += 0.05f;

			if (g_HPPortion[i].position.y > 0) {
				g_HPPortion[i].position.y -= 2;
			}
		}
	}

	HitCheckHPPortion();
}

//�@HP�|�[�V�����`�揈��
void DrawHPPortion() {

	for (int i = 0; i < HPPortion_Max; i++) {
		if (g_HPPortion[i].sts == 1) {
			// �f�o�C�X�R���e�L�X�g�擾
			ID3D11DeviceContext* devicecontext;
			devicecontext = Renderer::GetDeviceContext();

			// ���[���h�ϊ��s�񐶐�
			Matrix mtx;
			DX11MakeWorldMatrixRadian(
				mtx,
				g_HPPortion[i].scale,							// �X�P�[��
				g_HPPortion[i].rotation,							// �p��
				g_HPPortion[i].position);						// �ʒu

			// GPU�ɍs����Z�b�g����
			Renderer::SetWorldMatrix(&mtx);

			// �V�F�[�_�[��GPU�ɃZ�b�g
			g_shader.SetGPU();

			// ���f���`��
			g_staticmeshrenderer.Draw();
			//-------------------------------------//���`��
			// ���E�{�b�N�X�`��
			//mtx = g_obb.MakeWorldMtx(g_HPPortion.scale, mtx);
			//Renderer::SetWorldMatrix(&mtx);
			//g_material.SetGPU();
			//g_meshrenderer.Draw();
		}
	}

}

//�@HP�|�[�V�����I������
void ExitHPPortion() {
	g_staticmeshrenderer.Uninit();
}

//HP�|�[�V�����h���b�v
void HPPortionDrop(int idx) {
	
	for (int i = 0; i < HPPortion_Max; i++) {
		if (g_HPPortion[i].sts <= -1) {
			//�G�l�~�[������ł��茮�������Ă���Ȃ�
			if (GetItemEnemy(idx) && GetStsEnemy(idx) == -1) {
				g_HPPortion[i].position = GetEnemyPosition(idx);
				g_HPPortion[i].sts = 1;
				break;
			}
		}
	}

}

//�@HP�|�[�V���������h���b�v
void FallHPPotionDrop(int idx) {

	for (int i = 0; i < HPPortion_Max; i++) {
		if (g_HPPortion[i].sts <= -1) {
			//�G�l�~�[������ł��茮�������Ă���Ȃ�
			if (GetItemEnemy(idx) && GetStsEnemy(idx) == -1) {
				g_HPPortion[i].position.x = (rand() % (int)FieldX) - FieldX / 2;
				g_HPPortion[i].position.y = 400.0f;
				g_HPPortion[i].position.z = (rand() % (int)FieldZ) - FieldZ / 2;;
				g_HPPortion[i].sts = 1;
			}
			break;
		}
	}

}

//�@�v���C���[��HP�|�[�V�����̓����蔻��
void HitCheckHPPortion() {
	
	for (int i = 0; i < HPPortion_Max; i++) {
		if (g_HPPortion[i].sts == 1) {
			if (HitCheckOBB(GetPlayerOBB(), GetHPPortionOBB(i))) {
				if (NowBagCnt() >= BagMax) {
					g_HPPortion[i].sts = -1;
				}
				else
				{
					g_HPPortion[i].sts = -1;
					HPPortionCntUp();
					PlaySound(GetItemSE);
				}
			}
			break;
		}
	}

}