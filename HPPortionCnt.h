#pragma once

#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"camera.h"
#include	"Player.h"

//�@HP�|�[�V�����J�E���gUI��������
void InitHPPortionCnt();
//�@HP�|�[�V�����J�E���gUI�X�V����
void UpdateHPPortionCnt();
//�@HP�|�[�V�����J�E���gUI�`�揈��
void DrawHPPortionCnt();
//�@HP�|�[�V�����J�E���gUI�I������
void ExitHPPortionCnt();

//	����HP�|�[�V�����̏��������擾
void NowHPPortionCnt(int HPPortionCount);
