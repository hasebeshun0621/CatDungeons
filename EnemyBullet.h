#pragma once
#include	<directxmath.h>
#include	"renderer.h"
#include	"CPlane.h"
#include	"CMeshRenderer.h"
#include	"CBoxMesh.h"
#include	"CBoundingBox.h"

#include	"WaveScene.h"

#include	"Collision.h"

#include	"CatchPointer.h"
#include	"Enemy.h"

#include	"Field.h"

//�@�G�l�~�[�o���b�g�ő吔
constexpr int BulletEnemy_Max=200;

//�@�G�l�~�[�o���b�g���f���ő吔
constexpr int EnemyBulletMod_Max = 2;

//�@�G�l�~�[�ʃo���b�g�_���[�W
constexpr int ENEMYBULLETDAME_Sura = 15;
constexpr int ENEMYBULLETDAME_SuraLeaf = 10;

//�@�G�l�~�[�o���b�g�\����
struct EnemyBullet
{
	int sts = -1;									//���g�p-1�@�g�p���P

	int BulletType = 0;								//�X���C�����O�@�΃X���C����1

	DirectX::XMFLOAT3 position = { 0,0,0 };			//�ʒu
	DirectX::XMFLOAT3 rotation = { 0,0,0 };			//��]
	DirectX::XMFLOAT3 scale = { 1,1,1 };			//�T�C�Y
	DirectX::XMFLOAT3 vector = { 0,0,0 };			//�P�ʃx�N�g��

	float Speed = 0.0f;								//�e��
	int Dame = 0;									//�_���[�W

};

//�@�G�l�~�[�o���b�g��������
void InitEnemyBullet();
//�@�G�l�~�[�o���b�g�X�V����
void UpdateEnemyBullet();
//�@�G�l�~�[�o���b�g�`�揈��
void DrawEnemyBullet();
//�@�G�l�~�[�o���b�g�I������
void ExitEnemyBullet();

//�@�G�l�~�[�o���b�g�}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxEnemyBullet(int idx);

//�@�G�l�~�[�o���b�g�̃^�C�v���擾
int GetEnemyBulletType(int idx);
//�@�G�l�~�[�o���b�g�X�s�[�h�擾
float GetEnemyBulletSpeed(int idx);
//�@�G�l�~�[�o���b�g�_���[�W�擾
int GetEnemyBulletDame(int idx);

//�@�o���b�g�ƃl�b�g�̓����蔻��
bool HitCheckNet();
//�@�v���C���[�ƃG�l�~�[�o���b�g�̓����蔻��
void HitCheckPlayer_Enemy();
//�@�G�l�~�[�o���b�g���ˏ���
void FireEnemyBullet(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot,int type);
//�@�G�l�~�[�o���b�g�ړ�
void MoveEnemyBullet();
