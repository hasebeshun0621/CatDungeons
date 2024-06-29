#include	"Poison.h"

using namespace DirectX::SimpleMath;

// ���p
static CShader		g_shaderfield;

//  �����b�V���i�W�I���g���f�[�^�j
static CPlaneMesh g_planemesh;

// �����b�V���i�����_�����O�f�[�^�j
static CMeshRenderer g_meshrenderer;

// ���p�}�e���A��
static CMaterial	g_material;

// �v���C���̏��(S,R,T)
static DirectX::XMFLOAT3	g_position = { 0.0f,-70.0f,0.0f };		// �ʒu
static DirectX::XMFLOAT3	g_rotation = { 0.0f,0.0f,0.0f };		// �p��
static DirectX::XMFLOAT3	g_scale = { 1.0f,1.0f,1.0f };			// �g�k

// SRV
static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_srv;

//�@�ŏ���������
void InitPoison() {
	
	// �g�p����V�F�[�_�[�𐶐�
	g_shaderfield.SetShader(
		"shader/unlitTextureVS.hlsl",				// ���_�V�F�[�_
		"shader/unlitTexturePS.hlsl");				// �s�N�Z���V�F�[�_

	// �����b�V������
	g_planemesh.Init(
		15, 15,					// ������
		1000,					// �T�C�Yx
		1000,					// �T�C�Yz
		Color(1, 1, 1, 1),		// ���_�J���[
		Vector3(0, 0, -1),		// �@���x�N�g��
		true);					// XZ����

	// ���b�V�������_�������i���p�j
	g_meshrenderer.Init(g_planemesh);

	ID3D11Device* dev;
	ID3D11DeviceContext* devcontext;

	dev = Renderer::GetDevice();
	devcontext = Renderer::GetDeviceContext();

	bool sts = CreateSRVfromFile("assets/texture/maptile_dokunuma.png", dev, devcontext, &g_srv);
	if (!sts) {
		MessageBox(nullptr, "CreateSRVfromFile error(MeshWallInit)", "error", MB_OK);
	}

	g_position = { 0.0f,-70.0f,0.0f };		// �ʒu
	g_rotation = { 0.0f,0.0f,0.0f };		// �p��
	g_scale = { 1.0f,1.0f,1.0f };			// �g�k

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

//�@�ŏ��X�V����
void UpdatePoison(){

	static int fcnt = 0;
	static bool nur = false;

	if (fcnt < 100 && nur==false) {
		g_position.y += 0.05f;
		fcnt++;
	}
	else {
		nur = true;
	}

	if (fcnt > 0 && nur) {
		g_position.y -= 0.051f;
		fcnt--;
	}
	else {
		nur = false;
	}

}

//�@�ŏ��`�揈��
void DrawPoison() {

	// ���p�V�F�[�_�Z�b�g
	g_shaderfield.SetGPU();

	// ���p�}�e���A���Z�b�g
	g_material.SetGPU();

	// �e�N�X�`�����Z�b�g
	ID3D11DeviceContext* devcontext;
	devcontext = Renderer::GetDeviceContext();
	devcontext->PSSetShaderResources(0, 1, g_srv.GetAddressOf());

	// GPU�ɍs����Z�b�g����
	Matrix mtx;
	DX11MakeWorldMatrix(
		mtx,
		g_scale,							// �X�P�[��
		g_rotation,							// �p��
		g_position);						// �ʒu

	Renderer::SetWorldMatrix(&mtx);

	// ���`��
	g_meshrenderer.Draw();

}
