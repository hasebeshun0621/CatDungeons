#include	"CatchParticle.h"

using namespace DirectX;

using namespace DirectX::SimpleMath;

// �}�N����`
#define	TEXTURE_PARTICLE	"data/texture/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PARTICLE_SIZE_X		(50.0f)							// �r���{�[�h�̕�
#define	PARTICLE_SIZE_Y		(50.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_PARTICLE	(2.0f)							// �ړ����x

#define	MAX_PARTICLE		(10)							// �G�t�F�N�g�ő吔

#define MATERIAL_DIFFUSE	XMFLOAT4(1,1,1,1)
#define MATERIAL_AMBIENT	XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR	XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE	XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER		(0.0f)

// �p�[�e�B�N���\���̒�`
struct PARTICLE {
	XMFLOAT3 pos;			// �ʒu
	XMFLOAT3 rot;			// ��]
	XMFLOAT3 move;			// �ړ���
	XMFLOAT4 col;			// �F
	float fSizeX;			// ��
	float fSizeY;			// ����
	int nIdxShadow;			// �eID
	int nLife;				// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
};

// �v���g�^�C�v�錾
void SetVertexCatchParticle(int nIdxParticle, float fSizeX, float fSizeY);
void SetColorCatchParticle(int nIdxParticle, XMFLOAT4 col);

// �O���[�o���ϐ�
static CPlaneMesh		g_meshParticle;					// ���b�V�����
static CMaterial		g_material;						// �}�e���A��

static PARTICLE			g_aParticle[MAX_PARTICLE];		// �p�[�e�B�N�����[�N
static XMFLOAT3			g_posBase;						// �r���{�[�h�����ʒu

static float			g_fWidthBase = 5.0f;			// ��̕�
static float			g_fHeightBase = 3.0f;			// ��̍���
static bool				g_bPause = false;				// �|�[�YON/OFF

// SRV�i�e�N�X�`���j
static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_srv;

// �g�p����V�F�[�_
static CShader			g_shader;

// �����_��
static CMeshRenderer	g_meshrenderer;

//�@�L���b�`�p�[�e�B�N����������
void InitCatchParticle(){

	ID3D11Device* pDevice = Renderer::GetDevice();
	ID3D11DeviceContext* pDeviceContext = Renderer::GetDeviceContext();

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/unlitTextureVS.hlsl",				// ���_�V�F�[�_
		"shader/unlitTexturePS.hlsl");				// �s�N�Z���V�F�[�_

	// ���b�V������
	g_meshParticle.Init(
		1, 1,					// ������
		1.0f,					// �T�C�Y
		1.0f,					// �T�C�Y
		Color(1, 1, 1, 1),		// ���_�J���[
		Vector3(0, 0, -1),		// �@���x�N�g��
		false);					// XZ����

	// ���b�V�������_�������i���p�j
	g_meshrenderer.Init(g_meshParticle);

	ID3D11Device* dev;
	ID3D11DeviceContext* devcontext;

	dev = Renderer::GetDevice();
	devcontext = Renderer::GetDeviceContext();

	bool sts = CreateSRVfromFile("assets/1Get.png", dev, devcontext, &g_srv);
	if (!sts) {
		MessageBox(nullptr, "CreateSRVfromFile error(MeshWallInit)", "error", MB_OK);
	}


	// �}�e���A��������
	MATERIAL mtrl;
	mtrl.Ambient = Color(1, 1, 1, 1);
	mtrl.Diffuse = Color(1, 1, 1, 1);
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.TextureEnable = TRUE;
	g_material.Init(mtrl);

	// �p�[�e�B�N����񏉊���
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		g_aParticle[nCntParticle].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fSizeX = PARTICLE_SIZE_X;
		g_aParticle[nCntParticle].fSizeY = PARTICLE_SIZE_Y;
		g_aParticle[nCntParticle].nIdxShadow = -1;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;
	}

}


//�@�L���b�`�p�[�e�B�N���X�V����
void UpdateCatchParticle(){

	XMFLOAT3 rotCamera;

	// �J�����̉�]���擾
	rotCamera = Camera::GetRotCamera();

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		if (g_aParticle[nCntParticle].bUse) {
			// �g�p��
			g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;


			if (g_aParticle[nCntParticle].pos.y <= g_aParticle[nCntParticle].fSizeY / 2) {
				// ���n����
				g_aParticle[nCntParticle].pos.y = g_aParticle[nCntParticle].fSizeY / 2;
				//�o�E���h
				g_aParticle[nCntParticle].move.y = -g_aParticle[nCntParticle].move.y * 0.9f;
			}

			g_aParticle[nCntParticle].move.y -= 0.25f;

			g_aParticle[nCntParticle].nLife--;
			if (g_aParticle[nCntParticle].nLife <= 0) {
				g_aParticle[nCntParticle].bUse = false;
				g_aParticle[nCntParticle].nIdxShadow = -1;
			}
			else {
				//���񂾂�Ɠ����ɂ��č폜
				if (g_aParticle[nCntParticle].nLife <= 10) {
					g_aParticle[nCntParticle].col.x = 0.5f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
					g_aParticle[nCntParticle].col.y = 0.8f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
					g_aParticle[nCntParticle].col.z = 0.8f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
				}

				if (g_aParticle[nCntParticle].nLife <= 10) {
					// ���l�ݒ�
					g_aParticle[nCntParticle].col.w -= 0.05f;
					if (g_aParticle[nCntParticle].col.w < 0.0f) {
						g_aParticle[nCntParticle].col.w = 0.0f;
					}
				}

				// �F�̐ݒ�
				SetColorCatchParticle(nCntParticle, g_aParticle[nCntParticle].col);
			}
		}
	}

}

//�@�L���b�`�p�[�e�B�N���`�揈��
void DrawCatchParticle(){

	ID3D11DeviceContext* pDeviceContext = Renderer::GetDeviceContext();

	// �r���[�}�g���b�N�X���擾
	DirectX::SimpleMath::Matrix mtxView = Camera::GetViewMtx();

	// �p�[�e�B�N���p�V�F�[�_�Z�b�g
	g_shader.SetGPU();

	// �e�N�X�`�����Z�b�g
	ID3D11DeviceContext* devcontext;
	devcontext = Renderer::GetDeviceContext();
	devcontext->PSSetShaderResources(0, 1, g_srv.GetAddressOf());

	// �S�p�[�e�B�N�����r���{�[�h�Ƃ��ĕ`�悷��
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		if (g_aParticle[nCntParticle].bUse) {

			// ���[���h�}�g���b�N�X�̏�����
			Matrix mtxWorld = Matrix::Identity;

			// �r���{�[�h�p�s��쐬
			mtxWorld._11 = mtxView._11;
			mtxWorld._12 = mtxView._21;
			mtxWorld._13 = mtxView._31;
			mtxWorld._21 = mtxView._12;
			mtxWorld._22 = mtxView._22;
			mtxWorld._23 = mtxView._32;
			mtxWorld._31 = mtxView._13;
			mtxWorld._32 = mtxView._23;
			mtxWorld._33 = mtxView._33;

			// �X�P�[���s����쐬
			Matrix mtxScale = Matrix::CreateScale(g_aParticle[nCntParticle].fSizeX, g_aParticle[nCntParticle].fSizeY, 1.0f);

			// ���s�ړ��s����쐬
			Matrix mtxTranslate = Matrix::CreateTranslation(g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);

			// SRT����
			Matrix mtx = mtxScale * mtxWorld * mtxTranslate;

			// GPU�ɍs����Z�b�g����
			Renderer::SetWorldMatrix(&mtx);

			// �J���[�̐ݒ�
			g_material.SetDiffuse(g_aParticle[nCntParticle].col);

			// �}�e���A��
			MATERIAL mtrl;
			mtrl.Ambient = Color(1, 1, 1, 1);
			mtrl.Diffuse = g_aParticle[nCntParticle].col;
			mtrl.Emission = Color(0, 0, 0, 0);
			mtrl.Specular = Color(0, 0, 0, 0);
			mtrl.Shininess = 0;
			mtrl.TextureEnable = TRUE;

			g_material.SetMaterial(mtrl);

			// ���`��
			g_meshrenderer.Draw();

			// �|���`��
			//g_meshrenderer.Draw();
		}
	}

	// ������������
	Renderer::SetBlendState(1);
}

//�@�L���b�`�p�[�e�B�N���I������
void ExitCatchParticle(){

}

// ���_���W�̐ݒ�
void SetVertexCatchParticle(int nIdxParticle, float fSizeX, float fSizeY){

	if (nIdxParticle >= 0 && nIdxParticle < MAX_PARTICLE) {
		g_aParticle[nIdxParticle].fSizeX = fSizeX;
		g_aParticle[nIdxParticle].fSizeY = fSizeY;
	}

}

// ���_�J���[�̐ݒ�
void SetColorCatchParticle(int nIdxParticle, XMFLOAT4 col){

	if (nIdxParticle >= 0 && nIdxParticle < MAX_PARTICLE) {
		g_aParticle[nIdxParticle].col = col;
	}

}

// �L���b�`�G�t�F�N�g�̐ݒ�
int SetCatchParticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife){

	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		if (!g_aParticle[nCntParticle].bUse) {
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;

			// ���_���W�̐ݒ�
			SetVertexCatchParticle(nCntParticle, fSizeX, fSizeY);

			// ���_�J���[�̐ݒ�
			SetColorCatchParticle(nCntParticle, col);

			nIdxParticle = nCntParticle;


			break;
		}
	}

	return nIdxParticle;
}

void MakeCatchParticle(XMFLOAT3 pos) {

	std::mt19937 rnd;
	std::uniform_real_distribution<float> dist(0.0f, XM_2PI);
	
	//�p�[�e�B�N������
	XMFLOAT3 move;
	float fAngle, fLength;
	int nLife;
	float fSizeX;
	float fSizeY;

	fAngle = dist(rnd) - XM_PI;						// ���W�A��

	fLength = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;


	move.x = sinf(fAngle) * fLength;

	move.y = g_fHeightBase;

	move.z = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;

	//�G�t�F�N�g��������
	nLife = rand() % 5 + 40;
	//�G�t�F�N�g�T�C�Y�ݒ�
	fSizeX = 32.0f;
	fSizeY = 10.0f;
	pos.y = fSizeY / 2;

	//�J���[�̏����ݒ�
	SetCatchParticle(pos, move, XMFLOAT4(0.8f, 0.8f, 0.0f, 1.0f), fSizeX, fSizeY, nLife);

}
