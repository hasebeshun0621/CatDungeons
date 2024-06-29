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

//�@�{�X�o���b�g�ő吔
constexpr int BossBullet_Max = 200;
//�@�{�X�o���b�g�_���[�W
constexpr int ENEMYBULLETDAME_BOSS = 15;

//�@�{�X�o���b�g�\����
struct BossBullet
{

	int sts = -1;									//���g�p-1�@�g�p���P

	DirectX::XMFLOAT3 position = { 0,0,0 };			//�ʒu
	DirectX::XMFLOAT3 rotation = { 0,0,0 };			//��]
	DirectX::XMFLOAT3 scale = { 1,1,1 };			//�T�C�Y
	DirectX::XMFLOAT3 vector = { 0,0,0 };			//�P�ʃx�N�g��

	float Speed = 0.0f;								//�e��
	int Dame = 0;									//�_���[�W

};

//�@�{�X�o���b�g������
BossBullet InitializBossBullet();

//�@�{�X�o���b�g��������
void InitBossBullet();
//�@�{�X�o���b�g�X�V����
void UpdateBossBullet();
//�@�{�X�o���b�g�`�揈��
void DrawBossBullet();
//�@�{�X�o���b�g�I������
void ExitBossBullet();

//�@�{�X�o���b�g�}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxBossBullet(int idx);
//�@�{�X�o���b�g�_���[�W�擾
int GetBossBulletDame(int idx);

//�@�{�X�o���b�g�ƃv���C���[�̓����蔻��
void HitCheckPlayer_Boss();

//�@�{�X�o���b�g���ˏ���
void FireBossBullet(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot);

//�@�{�X�o���b�g�ړ�
void MoveBossBullet();

