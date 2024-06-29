#include	<functional>
#include	"scene.h"

// �J�����g�V�[���C���f�b�N�X
int g_CurrentSceneIdx = 0;
int BeforeScene = 0;
// ���݃V�[��
SceneFunctioTable	g_SceneTable[8] =
{
	// 0 �^�C�g���V�[��
	{
		InitTitleScene,
		UpdateTitleScene,
		DrawTitleScene,
		ExitTitleScene,
	},
	//1 �^�C�g�����j���[
	{
		InitTitleMenuScene,
		UpdateTitleMenuScene,
		DrawTitleMenuScene,
		ExitTitleMenuScene,
	},
	// 2 ��������V�[��(�`���[�g���A��)
	{
		InitTutorialScene,
		UpdateTutorialScene,
		DrawTutorialScene,
		ExitTutorialScene,
	},
	// 3 �Q�[���V�[��
	{
		InitWave,
		UpdateWave,
		DrawWave,
		ExitWave,
	},
	// 4 �{�X�Q�[���V�[��
	{
		InitBossScene,
		UpdateBossScene,
		DrawBossScene,
		ExitBossScene,
	},
	// 5 �Q�[���I�[�o�[�V�[��
	{
		InitGameOverScene , 
		UpdateGameOverScene,
		DrawGameOverScene,
		ExitGameOverScene,
	},
	// 6 �N���A�V�[��
	{
		InitGClear,
		UpdateGClear,
		DrawGClear,
		ExitGClear,
	},
	// 7 ���[�h�V�[��
	{
		InitRoadScene,
		UpdateRoadScene,
		DrawRoadScene,
		ExitRoadScene,
	},
};

// �V�[����������
void InitScene() {
	//�����_���V�[�h�ύX
	srand(time(NULL));

	g_SceneTable[g_CurrentSceneIdx].Init();

}

// �V�[���X�V����
void UpdateScene() {

	g_SceneTable[g_CurrentSceneIdx].Update();

}

// �V�[���`�揈��
void DrawScene() {

	g_SceneTable[g_CurrentSceneIdx].Draw();

}

// �V�[���I������
void ExitScene() {

	g_SceneTable[g_CurrentSceneIdx].Exit();

}

// �V�[���ԍ��Z�b�g
void SetScene(int idx) {

	if (g_CurrentSceneIdx != idx) {
		//�ЂƂO�̃V�[����ۊ�
		BeforeScene = GetScene();
	
		//g_SceneTable[g_CurrentSceneIdx].Exit();
		g_CurrentSceneIdx = idx;
		g_SceneTable[g_CurrentSceneIdx].Init();
	}

}

//�ЂƂO�̃V�[�����擾	
int GetBeforeScene() {

	return BeforeScene;

}

//���̃V�[�����擾
int GetScene() {

	int idx = g_CurrentSceneIdx;
	return idx;

}
