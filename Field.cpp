#include	"Field.h"

using namespace DirectX::SimpleMath;

// �V�F�[�_
static CShader		g_shaderfield;

//�@�p�l�����b�V��
static CPlaneMesh g_planemesh;

// �����b�V���i�����_�����O�f�[�^�j
static CMeshRenderer g_meshrenderer;

// ���p�}�e���A��
static CMaterial	g_material;

// �v���C���̏��(S,R,T)
static DirectX::XMFLOAT3	g_position = { 0.0f,0.0f,0.0f };		// �ʒu
static DirectX::XMFLOAT3	g_rotation = { 0.0f,0.0f,0.0f };		// �p��
static DirectX::XMFLOAT3	g_scale	   = { 1.0f,1.0f,1.0f };		// �g�k

// SRV
static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> g_srv;

// �t�B�[���h��������
void InitField() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shaderfield.SetShader(
		"shader/unlitTextureVS.hlsl",				// ���_�V�F�[�_
		"shader/unlitTexturePS.hlsl");				// �s�N�Z���V�F�[�_

	// �����b�V������
	g_planemesh.Init(
		10,8,					// ������
		FieldX,					// �T�C�Yx
		FieldZ,					// �T�C�Yz
		Color(1, 1, 1, 1),		// ���_�J���[
		Vector3(0, 0, -1),		// �@���x�N�g��
		true);					// XZ����

	// ���b�V�������_�������i���p�j
	g_meshrenderer.Init(g_planemesh);

	ID3D11Device* dev;
	ID3D11DeviceContext* devcontext;

	dev = Renderer::GetDevice();
	devcontext = Renderer::GetDeviceContext();

	bool sts = CreateSRVfromFile("assets/texture/maptile_sogen_02.png", dev, devcontext, &g_srv);
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

// �t�B�[���h�`�揈��
void DrawField() {

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

std::vector<VERTEX_3D> GetSquareVertices(int SquareNo) {

	std::vector<VERTEX_3D> vertices = g_planemesh.GetVertices();

	std::vector<unsigned  int> indices = g_planemesh.GetIndices();

	int idx0 = indices[SquareNo * 6+0];
	int idx1 = indices[SquareNo * 6+1];
	int idx2 = indices[SquareNo * 6+2];

	std::vector<VERTEX_3D> vlist;

	vlist.emplace_back(vertices[idx0]);
	vlist.emplace_back(vertices[idx1]);
	vlist.emplace_back(vertices[idx2]);

	idx0 = indices[SquareNo * 6 + 3];
	idx1 = indices[SquareNo * 6 + 4];
	idx2 = indices[SquareNo * 6 + 5];

	vlist.emplace_back(vertices[idx0]);
	vlist.emplace_back(vertices[idx1]);
	vlist.emplace_back(vertices[idx2]);

	return vlist;
}



//�G���A��O���� true�O	false��
bool FieldOutCheck(DirectX::XMFLOAT3 pos) {
	
	bool Check = false;
	if (pos.x < -FieldX / 2) {
		Check = true;
	}
	if (pos.x > FieldX / 2) {
		Check = true;
	}	
	if (pos.z > FieldZ / 2) {
		Check = true;
	}
	if (pos.z < -FieldZ / 2) {
		Check = true;
	}

	return Check;
}