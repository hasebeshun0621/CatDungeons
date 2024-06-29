#pragma once

#include	"XAudio2.h"
#include	"scene.h"
#include	"camera.h"

#include	"Player.h"
#include	"PlayerBullet.h"
#include	"CatchPointer.h"

#include	"Enemy.h"
#include	"EnemyBullet.h"

#include	"Key.h"
#include	"HPPortion.h"

#include	"field.h"
#include	"FieldWall.h"
#include	"wall.h"
#include	"Poison.h"

#include	"UI.h"
#include	"Score.h"
#include	"particle.h"

static int WaveMax = 3;

//�@�E�F�[�u�V�[����������
void InitWave();
//�@�E�F�[�u�V�[���X�V����
void UpdateWave();
//�@�E�F�[�u�V�[���`�揈��
void DrawWave();
//�@�E�F�[�u�V�[���I������
void ExitWave();

//�@�q�b�g�X�g�b�vFcnt��ݒ�
void SetHitStop(int cnt);
//�q�b�g�X�g�b�v��������
bool HitStop();

//�@�����E���Ǝ��̃X�e�[�W��
void NexWave();
//�@�{�X�E�F�[�u���`�F�b�N
bool NexBossCheck();
//�@�Q�[���I�[�o�[���`�F�b�N
void GameOverCheck();

//�@���̃E�F�[�u���擾
int GetNowWave();