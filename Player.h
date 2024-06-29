#pragma once
#include	<directxmath.h>
#include	"renderer.h"
#include	"CPlane.h"
#include	"CMeshRenderer.h"
#include	"CBoxMesh.h"
#include	"CBoundingBox.h"

#include	"HPPortion.h"
#include	"scene.h"
#include	"HPUI.h"
#include	"Field.h"
#include	"BodyParticle.h"

//�@�v���C���[�ő�HP
constexpr int PlayerHP_Max = 100;
// �v���C���[���A�r���e�B���g���ƌ���̗�
constexpr int PlayerAbilityDame = 30;
//�@�v���C���[�̃A�C�e�������ő吔
constexpr int BagMax = 5;

enum ItemName
{
	ABILITY=0,	//�A�r���e�B
	HEAL,		//��
	BULLET,		//�o���b�g
};

//�@�v���C���[�\����
struct Player {

	// �v���C���̏��(S,R,T)
	DirectX::SimpleMath::Vector3 position = { 0.0f,0.0f,0.0f };		// �ʒu
	DirectX::SimpleMath::Vector3 rotation = { 0.0f,0.0f,0.0f };		// �p��
	DirectX::SimpleMath::Vector3 scale = { 15.0f,15.0f,15.0f };		// �g�k

	float VALUE_MOVE_MODEL = 0.0f;						// �ړ��X�s�[�h
	float VALUE_ROTATE_MODEL = DirectX::XM_PI * 0.0f;	// ��]�X�s�[�h
	float RATE_ROTATE_MODEL = 0.0f;						// ��]�����W��
	float RATE_MOVE_MODEL = 0.0f;						// �ړ������W��

	bool HitSts = true;		//�����蔻��@false�Ȃ��@true����
	bool Control = true;	//�v���C���[����@false�Ȃ� true����
	bool EvasionSwitch = false;// ����I���I�t

	int ItemPointer = 0;	//�I�����Ă���\���L�[
	int ItemSideCnt = 0;	//���@�I��ł���A�C�e���ԍ�

	int HPPortionCnt = 0;	//�������Ă���|�[�V������

	int HP = 0;				//�̗�
	int ST = 0;				//�X�^�~�i
	int STcnt = 0;			//�X�^�~�i�񕜑��x

};

//�@�v���C���[��������
void InitPlayer();
//�@�v���C���[�X�V����
void UpdatePlayer();
//�@�v���C���[�`�揈��
void DrawPlayer();
//�@�v���C���[�I������
void ExitPlayer();

//�@�v���C���[����܂Ƃ�
void PlayerControl();
//�@�v���C���[�_�b�V��
void PlayerDash();
//�@�v���C���[���
void PlayerEvasion();
//�@�v���C���[����
bool PlayerFall();
//�@�v���C���[�|�W�V�������Z�b�g
void ResetPosPlayer();
//�@�v���C���[�m�b�N�o�b�N
bool PlayerKnockBack(int idx,int type);//type�@1�G�l�~�[�A2�{�X

//�v���C���[������
Player initializePlayer();

// �ʒu�擾
DirectX::XMFLOAT3 GetPositionPlayer();
// �p���擾
DirectX::XMFLOAT3 GetRotationPlayer();
//�T�C�Y�擾
DirectX::XMFLOAT3 GetScalePlayer();
// �v���C���[�}�g���N�X�擾
DirectX::SimpleMath::Matrix GetMtxPlayer();
// OBB�擾
BoundingBoxOBB GetPlayerOBB();
// �ړ��ʎ擾
DirectX::XMFLOAT3 GetMovePlayer();
// ��]�ڕW�p�x�擾
DirectX::XMFLOAT3 GetDestrotPlayer();

bool GetPlayerHitSts();
//�v���C���[HP�Q�b�g
int GetPlayerHp();
//�@HP�𑝂₷
void PlayerHPUP(int num);
//�@HP�����炷
void PlayerHPDOWN(int num);

//�v���C���[ST�Q�b�g
int GetPlayerSt();
//�@�v���C���[�X�^�~�i�����炷
void PlayerSTDOWN(int num);

//�@HP�|�[�V����������
int GetHPPortionCnt();
//�@HP�|�[�V�����𑝂₷
void HPPortionCntUp();
//�@HP�|�[�V���������炷
void HPPortionCntDown();

//�@���I��ł���A�C�e����
int GetItemPointer();
//�@���I��ł���o���b�g
int GetSelectBullet();

