#pragma once

#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"camera.h"
#include	"Player.h"

//�@�o���b�g�J�E���gUI��������
void InitBulletCnt();
//�@�o���b�g�J�E���gUI�X�V����
void UpdateBulletCnt();
//�@�o���b�g�J�E���gUI�`�揈��
void DrawBulletCnt();
//�@�o���b�g�J�E���gUI�I������
void ExitBulletCnt();

//	���̒e�̏��������擾
void NowBulletCnt(int Bulletcnt);