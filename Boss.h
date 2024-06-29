#pragma once
#include	<directxmath.h>
#include	"Collision.h"
#include	"renderer.h"

#include	"Player.h"
#include	"BossBullet.h"
#include	"Key.h"
#include	"HPPortion.h"
#include	"Field.h"

//�@�{�X�ő吔
constexpr int Boss_Max = 1;

//�@�{�X���f���ő吔
constexpr int BossMod_Max = 2;

//�@�{�X�̗�
constexpr int ENEMYHP_BOSS = 40;

//�@�{�X�\����
struct Boss
{
	int sts = 0;															//-1,���@�P������
	int Type = 0;															//�{�XTYPE
	DirectX::XMFLOAT3	position = { 0,0,0 };								//�ʒu
	DirectX::XMFLOAT3	rotation = { 0,0,0 };								//��]
	DirectX::XMFLOAT3	scale = { 0,0,0 };									//�T�C�Y
	DirectX::SimpleMath::Vector3	destrot = { 0.0f,0.0f,0.0f };			// �ڕW��]�p�x
	float RATE_ROTATE_ENEMY = 0.05f;										// ��]�����W��
	float MoveSpeed = 0.0f;													//�ړ����x

	int HP = 0;								//�̗�

	bool KeyGet = false;					//���������Ă��邩
	bool ItemGet = false;					//�A�C�e���������Ă��邩

	bool KnockSwitch = false;				//�m�b�N�o�b�N�I���I�t
	int Knockfcnt = 0;						//�m�b�N�o�b�N�J�E���g

	bool DeathSwitch = false;				//���S���o�I���I�t
	DirectX::SimpleMath::Matrix pbDeat;		//�q�b�g�����e�̐i�s�������擾

	int fcnt = 0;							//���˗p�J�E���^�[
	int fullcnt = 0;						//�e���ˊԊu

};

//�@�{�X������
Boss InitializBoss();

//�@�{�X��������
void InitBoss();
//�@�{�X�X�V����
void UpdateBoss();
//�@�{�X�`�揈��
void DrawBoss();
//�@�{�X�I������
void ExitBoss();

//�@�{�X�|�W�V�����擾
DirectX::XMFLOAT3 GetBossPosition();
//�@�{�X��]�擾
DirectX::XMFLOAT3 GetBossRotation();
//�@�{�X��]�ڕW���擾
DirectX::XMFLOAT3 GetBossDestrot();
//�@�{�X�}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxBoss();
//�@�{�XOBB�擾
BoundingBoxOBB GetBossOBB();

//�@�{�X�X�e�[�^�X�擾
int GetStsBoss();

//�@�{�X���v���C���[�Ɍ�����
void BossLookPlayer();
//�@�{�X�ړ�
void BossMove();
//�@�{�X�m�b�N�o�b�N
void BossKnockBack(int idx);
//�@�{�X����
void BossFall();

//�@�{�X��HP�����炷
void BossHPReduce(int dame);
//�@�{�X���S�`�F�b�N
void BossDeat(int bidx);
//�@�{�X���S���o
void BossDeathAnime();



