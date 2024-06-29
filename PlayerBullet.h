#pragma once
#include	<directxmath.h>
#include	"renderer.h"
#include	"CPlane.h"
#include	"CMeshRenderer.h"
#include	"CBoxMesh.h"
#include	"CBoundingBox.h"
#include	"Collision.h"

#include	"Score.h"
#include	"Player.h"
#include	"CatchPointer.h"
#include	"Field.h"

//�@�v���C���[�o���b�g�ő吔
constexpr int PlayerBullet_Max = 100;
//�@�v���C���[�o���b�g���f���ő吔
constexpr int PlayerBulletMod_Max = 3;

//�@�v���C���[�o���b�g�\����
struct PlayerBullet
{
	int sts = -1;	//���g�p-1�@�g�p���P

	int BulletType = 0;//������0�@�X���C���e=�P�X���C���Βe�@�� 2�@

	DirectX::XMFLOAT3 position = { 0,0,0 };			//�ʒu
	DirectX::XMFLOAT3 rotation = { 0,0,0 };			//��]
	DirectX::XMFLOAT3 scale = { 1,1,1 };			//�T�C�Y

	float SpeedBullet = 0.0f;						//�e��
	DirectX::XMFLOAT3 vector = { 0,0,0 };			//�o���b�g�x�N�g��

	int Dame = 0;									//�o���b�g�_���[�W

};

//�@�v���C���[�o���b�g��������
void InitPlayerBullet();
//�@�v���C���[�o���b�g�X�V����
void UpdatePlayerBullet();
//�@�v���C���[�o���b�g�`�揈��
void DrawPlayerBullet();
//�@�v���C���[�o���b�g�I������
void ExitPlayerBullet();

//�@�v���C���[�o���b�g�ʒu�擾
DirectX::XMFLOAT3 GetPositionPlayerBullet(int idx);
//  �o���b�g�}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxPlayerBullet(int idx);
//�@�v���C���[�o���b�gOBB�擾
BoundingBoxOBB GetPlayerBulletOBB(int idx);

//�@�e�����Ă�����`�F�b�N
bool CheckFirePlayerBullet(int idx, int Type);
//�@�o���b�g���ˏ���
void FirePlayerBullet(int Type);
//�@�v���C���[�o���b�g�ړ�
void MovePlayerBullet();
//�@�v���C���[�o���b�g�_���[�W�擾
int GetPlayerBulletDame(int idx);

//�@�v���C���[�o���b�g�ƃG�l�~�[�̓����蔻��
bool HitCheckEnemy(int idx);
//�@�v���C���[�o���b�g�ƃ{�X�̓����蔻��
bool HitCheckBoss();
