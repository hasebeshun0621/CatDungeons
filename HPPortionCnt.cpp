#include	"HPPortionCnt.h"

using namespace DirectX::SimpleMath;

// �V�F�[�_�[
static CShader g_shader;

// HP�|�[�V����������
static int g_HPPortionCount = 0;

// ���_�o�b�t�@
static ID3D11Buffer* g_VertexBuffer{};

// �e�N�X�`�����\�[�X
static ID3D11ShaderResourceView* g_Texture{};

// �}�e���A��
static CMaterial	g_material{};

//�@HP�|�[�V�����J�E���gUI��������
void InitHPPortionCnt() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/unlitTextureVS.hlsl",				// ���_�V�F�[�_
		"shader/unlitTexturePS.hlsl");				// �s�N�Z���V�F�[�_

	// �e�N�X�`���ǂݍ���
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		L"assets/texture/number.png",
		nullptr,
		&g_Texture);

	g_HPPortionCount = 0;

	assert(g_Texture);

	// ���_�o�b�t�@����
	VERTEX_3D vertex[4];

	vertex[0].Position = Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3(200.0f, 0.0f, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3(0.0f, 200.0f, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3(200.0f, 200.0f, 0.0f);
	vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);

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

//�@HP�|�[�V�����J�E���gUI�X�V����
void UpdateHPPortionCnt() {

	NowHPPortionCnt(GetHPPortionCnt());

}

//�@HP�|�[�V�����J�E���gUI�`�揈��
void DrawHPPortionCnt() {

	// 2D�`��p�̃J�����������s
	Camera::Draw2D();

	// �V�F�[�_�[��GPU�ɃZ�b�g
	g_shader.SetGPU();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	g_material.SetGPU();

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	int count = g_HPPortionCount;

	for (int i = 0; i < 2; i++)
	{
		int number = count % 10;
		count /= 10;

		//�e�N�X�`�����W�Z�o
		float tx = number % 5 * (1.0f / 5);
		float ty = number / 5 * (1.0f / 5);

		//���_�f�[�^��������
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(g_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		float y = SCREEN_HEIGHT - 60;
		float x = SCREEN_WIDTH - 120 - i * 25.0f;
		float height = 45.0f;
		float width = 45.0f;

		vertex[0].Position = Vector3(x, y, 0.0f);
		vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
		vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = Vector2(tx, ty);

		vertex[1].Position = Vector3(x + width, y, 0.0f);
		vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = Vector2(tx + 0.2f, ty);

		vertex[2].Position = Vector3(x, y + height, 0.0f);
		vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
		vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = Vector2(tx, ty + 0.2f);

		vertex[3].Position = Vector3(x + width, y + height, 0.0f);
		vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
		vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = Vector2(tx + 0.2f, ty + 0.2f);

		Renderer::GetDeviceContext()->Unmap(g_VertexBuffer, 0);

		// �|���S���`��
		Renderer::GetDeviceContext()->Draw(4, 0);
	}

}

//�@HP�|�[�V�����J�E���gUI�I������
void ExitHPPortionCnt() {
	g_VertexBuffer->Release();
	g_material.Uninit();
	g_Texture->Release();
}

//	����HP�|�[�V�����̏��������擾
void NowHPPortionCnt(int HPPortionCount) {
	g_HPPortionCount = HPPortionCount;
}
