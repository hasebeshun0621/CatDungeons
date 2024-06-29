#include	"FieldWall.h"

using namespace DirectX::SimpleMath;

// �Ǘp
static CShader		g_shader;

//  �ǃ��b�V���i�W�I���g���f�[�^�j
static CPlaneMesh g_planemesh;

// �ǃ��b�V���i�����_�����O�f�[�^�j
static CMeshRenderer g_meshrenderer[FieldWall_Max];

// �Ǘp�}�e���A��
static CMaterial	g_material;

// �ǂ̏��(S,R,T)
struct DirectX::SimpleMath::Vector3 FieldWallPosTable[FieldWall_Max] =//�ʒu
{
	{FieldX / 2,	-FieldX / 2,	0},
	{0,			-FieldZ / 2,	FieldZ / 2},
	{-FieldX / 2,	-FieldX / 2,	0},
	{0,			-FieldZ / 2,	-FieldZ / 2},
};

struct DirectX::SimpleMath::Vector3 FieldWallRotTable[FieldWall_Max] =// �p�� 
{
	{ 180.0f,	 0,	90.0f},
	{ 90.0f,	0,		0},
	{ 180.0f,	0,	-90.0f},
	{-90.0f,	0,		0},
};

static DirectX::XMFLOAT3	g_scale = { 1.0f,1.0f,1.0f };			// �g�k

// SRV
static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_srv;

//�@�t�B�[���h�Ǐ�������
void InitFieldWall() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/unlitTextureVS.hlsl",				// ���_�V�F�[�_
		"shader/unlitTexturePS.hlsl");				// �s�N�Z���V�F�[�_

	// �����b�V������
	g_planemesh.Init(
		8, 8,					// ������
		FieldX,					// �T�C�Y
		FieldZ,					// �T�C�Y
		Color(1, 1, 1, 1),		// ���_�J���[
		Vector3(0, 0, -1),		// �@���x�N�g��
		true);					// XZ����

	for (int i = 0; i < FieldWall_Max; i++) {
		// ���b�V�������_�������i���p�j
		g_meshrenderer[i].Init(g_planemesh);
	}

	ID3D11Device* dev;
	ID3D11DeviceContext* devcontext;

	dev = Renderer::GetDevice();
	devcontext = Renderer::GetDeviceContext();

	bool sts = CreateSRVfromFile("assets/texture/maptile_ishizukuri_gray_02.png", dev, devcontext, &g_srv);

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

}

//�@�t�B�[���h�Ǖ`�揈��
void DrawFieldWall() {

	// �Ǘp�V�F�[�_�Z�b�g
	g_shader.SetGPU();

	// �Ǘp�}�e���A���Z�b�g
	g_material.SetGPU();

	// �e�N�X�`�����Z�b�g
	ID3D11DeviceContext* devcontext;
	devcontext = Renderer::GetDeviceContext();
	devcontext->PSSetShaderResources(0, 1, g_srv.GetAddressOf());

	// GPU�ɍs����Z�b�g����
	Matrix mtx;
	for (int i = 0; i < FieldWall_Max; i++) {

		DX11MakeWorldMatrix(
			mtx,
			g_scale,							// �X�P�[��
			FieldWallRotTable[i],				// �p��
			FieldWallPosTable[i]);				// �ʒu

		Renderer::SetWorldMatrix(&mtx);
	
		// �Ǖ`��
		g_meshrenderer[i].Draw();
	}

}


