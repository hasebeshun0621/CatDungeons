//#include "TitleMenuUI_.h"
//
//using namespace DirectX::SimpleMath;
//
//TMUI_a g_TMUIa;
//
//// �`��ׂ̈ɕK�v�ȏ��
//// �g�p����V�F�[�_�[
//static CShader		g_shader;
//
//// RTS���
////static DirectX::SimpleMath::Vector3	g_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
////static DirectX::SimpleMath::Vector3	g_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
////static DirectX::SimpleMath::Vector3	g_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
//
//// ���_�o�b�t�@
//static ID3D11Buffer* g_VertexBuffer{};
//
//// �e�N�X�`�����\�[�X
//static ID3D11ShaderResourceView* g_Texture{};
//
//// �}�e���A��
//static CMaterial	g_material{};
//
//
//void InitTMUI_a() {
//
//	// �g�p����V�F�[�_�[�𐶐�
//	g_shader.SetShader(
//		"shader/unlitTextureVS.hlsl",				// ���_�V�F�[�_
//		"shader/unlitTexturePS.hlsl");				// �s�N�Z���V�F�[�_
//
//	// �e�N�X�`���ǂݍ���
//	DirectX::CreateWICTextureFromFile(
//		Renderer::GetDevice(),
//		L"assets/mada.png",
//		nullptr,
//		&g_Texture);
//
//	assert(g_Texture);
//
//	// ���_�o�b�t�@����
//	VERTEX_3D vertex[4];
//
//	vertex[0].Position = Vector3(SCREEN_WIDTH / 3 + g_TMUIa.X_L, g_TMUIa.top + g_TMUIa.Y_U, -1.0f);
//	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
//	vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[0].TexCoord = Vector2(0.0f, 0.0f);
//
//	vertex[1].Position = Vector3((SCREEN_WIDTH / 3) * 2 + g_TMUIa.X_R, g_TMUIa.top + g_TMUIa.Y_U, -1.0f);
//	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
//	vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[1].TexCoord = Vector2(1.0f, 0.0f);
//
//	vertex[2].Position = Vector3(SCREEN_WIDTH / 3 + g_TMUIa.X_L, g_TMUIa.under + g_TMUIa.Y_D, -1.0f);
//	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
//	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[2].TexCoord = Vector2(0.0f, 1.0f);
//
//	vertex[3].Position = Vector3((SCREEN_WIDTH / 3) * 2 + g_TMUIa.X_R, g_TMUIa.under + g_TMUIa.Y_D, -1.0f);
//	vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
//	vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[3].TexCoord = Vector2(1.0f, 1.0f);
//
//
//	// ���_�o�b�t�@����
//	D3D11_BUFFER_DESC bd;
//	ZeroMemory(&bd, sizeof(bd));
//	bd.Usage = D3D11_USAGE_DYNAMIC;
//	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//
//	D3D11_SUBRESOURCE_DATA sd;
//	ZeroMemory(&sd, sizeof(sd));
//	sd.pSysMem = vertex;
//
//	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);
//
//	// �}�e���A��������
//	MATERIAL mtrl;
//	mtrl.Ambient = Color(1, 1, 1, 1);
//	mtrl.Diffuse = Color(1, 1, 1, 1);
//	mtrl.Emission = Color(0, 0, 0, 0);
//	mtrl.Specular = Color(0, 0, 0, 0);
//	mtrl.Shininess = 0;
//	mtrl.TextureEnable = TRUE;
//
//	g_material.Init(mtrl);
//}
//
//void UpdateTMUI_a() {
//	if (GetSelect() == 2) {
//		if (g_TMUIa.X_L > -10) {
//			g_TMUIa.X_L--;
//			g_TMUIa.X_R++;
//			g_TMUIa.Y_U--;
//			g_TMUIa.Y_D++;
//		}
//	}
//	else {
//		g_TMUIa.X_L = 0;
//		g_TMUIa.X_R = 0;
//		g_TMUIa.Y_U = 0;
//		g_TMUIa.Y_D = 0;
//	}
//}
//
//void DrawTMUI_a() {
//	// �V�F�[�_�[��GPU�ɃZ�b�g
//	g_shader.SetGPU();
//
//	// ���_�o�b�t�@�ݒ�
//	UINT stride = sizeof(VERTEX_3D);
//	UINT offset = 0;
//	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);
//
//	// �}�e���A���ݒ�
//	g_material.SetGPU();
//
//	// �e�N�X�`���ݒ�
//	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
//
//	// �v���~�e�B�u�g�|���W�ݒ�
//	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//
//	//���_�f�[�^��������
//	D3D11_MAPPED_SUBRESOURCE msr;
//	Renderer::GetDeviceContext()->Map(g_VertexBuffer, 0,
//		D3D11_MAP_WRITE_DISCARD, 0, &msr);
//
//	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
//
//	vertex[0].Position = Vector3(SCREEN_WIDTH / 3 + g_TMUIa.X_L, g_TMUIa.top + g_TMUIa.Y_U, 0.0f);
//	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
//	vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[0].TexCoord = Vector2(0.0f, 0.0f);
//
//	vertex[1].Position = Vector3((SCREEN_WIDTH / 3) * 2 + g_TMUIa.X_R, g_TMUIa.top + g_TMUIa.Y_U, 0.0f);
//	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
//	vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[1].TexCoord = Vector2(1.0f, 0.0f);
//
//	vertex[2].Position = Vector3(SCREEN_WIDTH / 3 + g_TMUIa.X_L, g_TMUIa.under + g_TMUIa.Y_D, 0.0f);
//	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
//	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[2].TexCoord = Vector2(0.0f, 1.0f);
//
//	vertex[3].Position = Vector3((SCREEN_WIDTH / 3) * 2 + g_TMUIa.X_R, g_TMUIa.under + g_TMUIa.Y_D, 0.0f);
//	vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
//	vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
//	vertex[3].TexCoord = Vector2(1.0f, 1.0f);
//
//	Renderer::GetDeviceContext()->Unmap(g_VertexBuffer, 0);
//
//	// �|���S���`��
//	Renderer::GetDeviceContext()->Draw(4, 0);
//}
//
//void ExitTMUI_a() {
//	g_VertexBuffer->Release();
//	g_material.Uninit();
//	g_Texture->Release();
//}
//
//
//
