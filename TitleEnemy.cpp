#include	"TitleEnemy.h"

using namespace DirectX::SimpleMath;

//�@�^�C�g���p�G�l�~�[�\����
TitleEnemy g_enemy;

// �V�F�[�_�[
static CShader		g_shader;

// �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
static CStaticMesh g_staticmesh[EnemyMod_Max];

// ���b�V�������_���[
static CStaticMeshRenderer g_staticmeshrenderer[EnemyMod_Max];

//�@�^�C�g���p�G�l�~�[��������
void InitTitleEnemy() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// ���_�V�F�[�_
		"shader/vertexLightingPS.hlsl");				// �s�N�Z���V�F�[�_

	// ���f���t�@�C����
	std::string filename[] = {
		u8"assets\\model\\sura.obj",
		u8"assets\\model\\suragreen.obj",
	};

	g_enemy.position = { -270.0f,0.0f,-90.0f };		// �ʒu
	g_enemy.rotation = { 0.0f,0.0f,0.0f };			// �p��
	g_enemy.scale = { 15.0f,15.0f,15.0f };			// �g�k

	for (int i = 0; i < EnemyMod_Max; i++) {

		// ���b�V�������i�W�I���g���f�[�^�j
		g_staticmesh[i].Init(filename[i]);

		// �`��ׂ̈̃f�[�^����
		g_staticmeshrenderer[i].Init(g_staticmesh[i]);
	}

	//�v���C���[���f�����E�Ɍ�����
	g_enemy.rotation.y = -DirectX::XM_PI * 0.50f;

}

//�@�^�C�g���p�G�l�~�[�X�V����
void UpdateTitleEnemy() {

	if (GetScene() == TITLE) {
		g_enemy.position.x++;	
		if (g_enemy.position.x >= 200) {
			g_enemy.position.x = -200;
			g_enemy.enetype++;
		}
	}

	if (GetScene() == TITLEMENU) {
		g_enemy.position.x--;
		g_enemy.rotation.y = DirectX::XM_PI * 0.50f;
		if (g_enemy.position.x <= -200) {
			g_enemy.position.x = 200;
			g_enemy.enetype++;
		}
	}

	if (g_enemy.enetype == EnemyMod_Max) {
		g_enemy.enetype = 0;
	}
}

//�@�^�C�g���p�G�l�~�[�`�揈��
void DrawTitleEnemy() {

	//�J�����`�揈��
	Camera::Draw();
	
	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ���[���h�ϊ��s�񐶐�
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_enemy.scale,							// �X�P�[��
		g_enemy.rotation,							// �p��
		g_enemy.position);						// �ʒu

	// GPU�ɍs����Z�b�g����
	Renderer::SetWorldMatrix(&mtx);

	// �V�F�[�_�[��GPU�ɃZ�b�g
	g_shader.SetGPU();

	// ���f���`��
	g_staticmeshrenderer[g_enemy.enetype].Draw();

}

//�@�^�C�g���p�G�l�~�[�I������
void ExitTitleEnemy() {

	for (int i = 0; i < EnemyMod_Max; i++) {
		g_staticmeshrenderer[i].Uninit();
	}

}

