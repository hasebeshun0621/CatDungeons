#include "TitleMenuUI_game.h"

using namespace DirectX::SimpleMath;

//�@�^�C�g�����j���[_�Q�[���\����
TitleMenuUI_game g_TitleMenuUI_g;

// �V�F�[�_�[
static CShader		g_shader;

// ���_�o�b�t�@
static ID3D11Buffer* g_VertexBuffer{};

// �e�N�X�`�����\�[�X
static ID3D11ShaderResourceView* g_Texture{};

// �}�e���A��
static CMaterial	g_material{};

//�@�^�C�g�����j���[_�Q�[����������
void InitTitleMenuUI_game() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/unlitTextureVS.hlsl",				// ���_�V�F�[�_
		"shader/unlitTexturePS.hlsl");				// �s�N�Z���V�F�[�_

	// �e�N�X�`���ǂݍ���
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		L"assets/TitleMenuUI_starat2.png",
		nullptr,
		&g_Texture);

	assert(g_Texture);

	// ���_�o�b�t�@����
	VERTEX_3D vertex[4];

	vertex[0].Position = Vector3(SCREEN_WIDTH / 3 + g_TitleMenuUI_g.X_L, g_TitleMenuUI_g.top + g_TitleMenuUI_g.Y_U, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3((SCREEN_WIDTH / 3) * 2 + g_TitleMenuUI_g.X_R, g_TitleMenuUI_g.top + g_TitleMenuUI_g.Y_U, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3(SCREEN_WIDTH / 3 + g_TitleMenuUI_g.X_L, g_TitleMenuUI_g.under + g_TitleMenuUI_g.Y_D, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3((SCREEN_WIDTH / 3) * 2 + g_TitleMenuUI_g.X_R, g_TitleMenuUI_g.under + g_TitleMenuUI_g.Y_D, 0.0f);
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

//�@�^�C�g�����j���[_�Q�[���X�V����
void UpdateTitleMenuUI_game() {
	
	if (GetSelect() == 0) {
		if (g_TitleMenuUI_g.X_L > -10) {
			g_TitleMenuUI_g.X_L--;
			g_TitleMenuUI_g.X_R++;
			g_TitleMenuUI_g.Y_U--;
			g_TitleMenuUI_g.Y_D++;
		}
	}
	else {
		g_TitleMenuUI_g.X_L = 0;
		g_TitleMenuUI_g.X_R = 0;
		g_TitleMenuUI_g.Y_U = 0;
		g_TitleMenuUI_g.Y_D = 0;
	}

}

//�@�^�C�g�����j���[_�Q�[���`�揈��
void DrawTitleMenuUI_game() {

	//�@�J����2D�`�揈��
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

	//���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(g_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = Vector3(SCREEN_WIDTH / 3 + g_TitleMenuUI_g.X_L, g_TitleMenuUI_g.top + g_TitleMenuUI_g.Y_U, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3((SCREEN_WIDTH / 3) * 2 + g_TitleMenuUI_g.X_R, g_TitleMenuUI_g.top + g_TitleMenuUI_g.Y_U, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3(SCREEN_WIDTH / 3 + g_TitleMenuUI_g.X_L, g_TitleMenuUI_g.under + g_TitleMenuUI_g.Y_D, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3((SCREEN_WIDTH / 3) * 2 + g_TitleMenuUI_g.X_R, g_TitleMenuUI_g.under + g_TitleMenuUI_g.Y_D, 0.0f);
	vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);

}

//�@�^�C�g�����j���[_�Q�[���I������
void ExitTitleMenuUI_game() {

	g_VertexBuffer->Release();
	g_material.Uninit();
	g_Texture->Release();

}



