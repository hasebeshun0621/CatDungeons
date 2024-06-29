#pragma once
#include	<directxmath.h>
#include	"Collision.h"

#include	"Player.h"
#include	"EnemyBullet.h"
#include	"Key.h"
#include	"HPPortion.h"
#include	"Field.h"

//�@�G�l�~�[�ő吔
constexpr int Enemy_Max = 100;

//�@Sura��HP�ݒ�
constexpr int ENEMYHP_Sura = 20;
//�@Suragreen��HP�ݒ�
constexpr int ENEMYHP_SuraGreen = 30;

enum EnemyMod
{
	Sura = 0,		//�X���C��
	SraGreen,		//�O���[���X���C��
	EnemyMod_Max
};

//�@�G�l�~�[�\����
struct Enemy
{
	int sts = 0;															//�@-1���@1������

	int EnemyType = 0;														//�@�G�l�~�[�^�C�v����

	DirectX::XMFLOAT3	position = { 0,0,0 };								//�ʒu
	DirectX::XMFLOAT3	rotation = { 0,0,0 };								//��]
	DirectX::XMFLOAT3	scale	 = { 0,0,0 };								//�T�C�Y
	DirectX::SimpleMath::Vector3	destrot = { 0.0f,0.0f,0.0f };			// �ڕW��]�p�x
	float RATE_ROTATE_ENEMY = 0.05f;										// ��]�����W��

	int HP = 0;								//�G�l�~�[�̗�
	bool KeyGet = false;					//���������Ă��邩
	bool ItemGet = false;					//�A�C�e���������Ă��邩

	bool KnockSwitch = false;				//�m�b�N�o�b�N�I���I�t
	int Knockfcnt = 0;						//�m�b�N�o�b�N�J�E���g
	
	bool DeathSwitch = false;				//���S���o�I���I�t
	DirectX::SimpleMath::Matrix pbDeat;		//�q�b�g�����e�̐i�s�������擾

	int fcnt = 0;							//���˗p�J�E���^�[
	int fullcnt = 0;						//�e���ˊԊu

};

//�@�G�l�~�[��������
void InitEnemy(int idx);
//�@�G�l�~�[�X�V����
void UpdateEnemy();
//�@�G�l�~�[�`�揈��
void DrawEnemy();
//�@�G�l�~�[�I������
void ExitEnemy();

//�@�G�l�~�[��idx�Ԗڂ̈ʒu�擾
DirectX::XMFLOAT3 GetEnemyPosition(int idx);
//�@�G�l�~�[��idx�Ԗڂ̉�]�擾
DirectX::XMFLOAT3 GetEnemyRotation(int idx);
//�@�G�l�~�[��idx�Ԗڂ̉�]�ڕW���擾
DirectX::XMFLOAT3 GetEnemyDestrot(int idx);
//�@�G�l�~�[��idx�Ԗڂ̃}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxEnemy(int idx);
//�@�G�l�~�[��idx�Ԗڂ�OBB�擾
BoundingBoxOBB GetEnemyOBB(int idx);

//�@�G�l�~�[�X�e�[�^�X���擾
int GetStsEnemy(int idx);
//�@�G�l�~�[�^�C�v�擾
int GetEnemyType(int idx);
//�@���������Ă��邩�`�F�b�N
bool GetKeyEnemy(int idx);
//�@�A�C�e���������Ă��邩�`�F�b�N
bool GetItemEnemy(int idx);

//�@�G�l�~�[���v���C���[�Ɍ�����
void EnemyLookPlayer();
//�@�G�l�~�[��HP�����炷
void EnemyHPReduce(int idx,int dame);
//�@�G�l�~�[�m�b�N�o�b�N
void EnemyKnockBack(int bulletucnt,int idx);
//�@�G�l�~�[����
void EnemyFall();

//�@�G�l�~�[���S�`�F�b�N
void EnemyDeat(int idx, int bidx);
//�@�G�l�~�[���S���o
void EnemyDeathAnime();




