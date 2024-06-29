#pragma once
#include	<directxmath.h>
#include	"renderer.h"
#include	"CPlane.h"
#include	"CMeshRenderer.h"

#include	"Player.h"
#include	"PlayerBullet.h"
#include	"EnemyBullet.h"
#include	"Field.h"

enum PointerSts {
	POINTER,	//�|�C���^
	NET,		//�l�b�g
	Model_Max,	//���f���ő吔
};

//�@�|�C���^�\����
struct Pointer{

	int sts =POINTER;							//��� 0�@�L���b�`�P

	DirectX::XMFLOAT3 position = { 0,0,0 };		//�ʒu
	DirectX::XMFLOAT3 rotation = { 0,0,0 };		//��]
	DirectX::XMFLOAT3 scale	   = { 1,1,1 };		//�T�C�Y

	DirectX::XMFLOAT3 vector = { 0,0,0 };		//�P�ʃx�N�g��

	int CatchCum = 0;							//�e���E�������v��
	int CatchBullet_Sura = 0;					//sura�e��
	int CatchBullet_SuraG = 0;					//suragreen�e��

	bool CatchFlag = false;						//�l�b�g�I���I�t
	int Catchfcnt = 0;							//�l�b�g���L�����^�C��
	int CatchReuse = 0;							//�l�b�g�Ďg�p�^�C��

};

//�@�|�C���^��������
void InitPointer();
//�@�|�C���^�X�V����
void UpdatePointer();
//�@�|�C���^�`�揈��
void DrawPointer();
//�@�|�C���^�I������
void ExitPointer();

//�@�|�C���^OBB�擾
BoundingBoxOBB GetPointerOBB();

//�@�|�C���^�X�e�[�^�X���擾
int GetStsPointer();
//�@�o���b�g�L���b�`���v���擾
int GetCatchCnt();
//�@�^�C�v�ʃL���b�`���擾
int GetCatchBullet(int Type);
//�@�o���b�g���𑝂₷
void CatchCntUp(int Type);
//�@�o���b�g�������炷
void CatchCntDown(int Type);

//�@�l�b�g��U��
void MoveNet(bool flag);
