#include	"TitlePlayer.h"

using namespace DirectX::SimpleMath;

// �V�F�[�_�[
static CShader		g_shader;

// �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
static CStaticMesh g_staticmesh;

// ���b�V�������_���[
static CStaticMeshRenderer g_staticmeshrenderer;

// RTS���
Vector3 g_position = { -200.0f,0.0f,0.0f };		// �ʒu
Vector3 g_rotation = { 0.0f,0.0f,0.0f };		// �p��
Vector3 g_scale = { 15.0f,15.0f,15.0f };		// �g�k

//�@�^�C�g���p�v���C���[��������
void InitTitlePlayer() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// ���_�V�F�[�_
		"shader/vertexLightingPS.hlsl");				// �s�N�Z���V�F�[�_

	// ���f���t�@�C����
	std::string filename[] = {
		u8"assets\\model\\neko.obj"
	};

	g_position = { -200.0f,0.0f,-90.0f };		// �ʒu
	g_rotation = { 0.0f,0.0f,0.0f };			// �p��
	g_scale = { 15.0f,15.0f,15.0f };			// �g�k

	// ���b�V�������i�W�I���g���f�[�^�j
	g_staticmesh.Init(filename[0]);

	// �`��ׂ̈̃f�[�^����
	g_staticmeshrenderer.Init(g_staticmesh);

	//�v���C���[���f�����E�Ɍ�����
	g_rotation.y = -DirectX::XM_PI * 0.50f;

}

//�@�^�C�g���p�v���C���[�X�V����
void UpdateTitlePlayer() {
	
	if (GetScene() == TITLE) {
		g_position.x++;
		if (g_position.x >= 200) {
			g_position.x = -200;
		}
	}

	if (GetScene() == TITLEMENU) {
		g_position.x--;
		g_rotation.y = DirectX::XM_PI * 0.50f;
		if (g_position.x <= -200) {
			g_position.x = 200;
		}
	}
}

//�@�^�C�g���p�v���C���[�`�揈��
void DrawTitlePlayer() {

	//�J�����`�揈��
	Camera::Draw();

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ���[���h�ϊ��s�񐶐�
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_scale,							// �X�P�[��
		g_rotation,							// �p��
		g_position);						// �ʒu

	// GPU�ɍs����Z�b�g����
	Renderer::SetWorldMatrix(&mtx);

	// �V�F�[�_�[��GPU�ɃZ�b�g
	g_shader.SetGPU();

	// ���f���`��
	g_staticmeshrenderer.Draw();

}

//�@�^�C�g���p�v���C���[�I������
void ExitTitlePlayer() {

	g_staticmeshrenderer.Uninit();

}

