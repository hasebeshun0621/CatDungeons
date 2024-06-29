#pragma once
#include	<functional>

#include	"TitleScene.h"
#include	"TitleMenuScene.h"
#include	"TutorialScene.h"
#include	"WaveScene.h"
#include	"BossScene.h"
#include	"RoadScene.h"
#include	"GameOverScene.h"
#include	"GameClear.h"

// �V�[�����񋓃N���X
enum GAMESCENE 
{
	TITLE,			//�@�^�C�g��
	TITLEMENU,		//�@�^�C�g�����j���[
	TUTORIAL,		//�@�`���[�g���A��
	GAME,			//�@�Q�[��wave
	BOSS,			//�@�{�X��
	GAMEOVER,		//�@�Q�[���I�[�o�[
	GAMECLEAR,		//�@�Q�[���N���A
	ROAD,			//�@���[�h���
	SCENEMAX,		//�@�V�[��Max
};

// �V�[���e�[�u��
struct SceneFunctioTable {
	std::function<void(void)> Init;		// ��������
	std::function<void(void)> Update;	// �X�V����
	std::function<void(void)> Draw;		// �`�揈��
	std::function<void(void)> Exit;		// �I������
};

// �V�[����������
void InitScene();
// �V�[���X�V����
void UpdateScene();
// �V�[���`�揈��
void DrawScene();
// �V�[���I������
void ExitScene();
// �V�[���Z�b�g
void SetScene(int idx);

//�ЂƂO�̃V�[�����擾	
int GetBeforeScene();
//���̃V�[�����擾
int GetScene();
