//=============================================================================
//
// �T�E���h���� [XAudio2.h]
//
//=============================================================================
#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include <xaudio2.h>

// �T�E���h�t�@�C��
typedef enum
{
	TitleBGM=0,					// �^�C�g��BGM
	WaveBGM,					// �o�g���V�[��BGM
	BossBGM,					// �{�X�V�[��BGM
	CatchSE,					// �o���b�g�L���b�`SE
	HitSE,						// �e����������SE
	KOSE,						// �G��|��SE
	DameSE,						// �e���������SE
	FireSE,						// �e������SE
	DashSE,						// �_�b�V��SE
	FallSE,						// ����SE
	HealSE,						// ��SE
	ItemSelectSE,				// �A�C�e���I��SE
	BulletSE,					// �o���b�g�I��SE
	GetKeySE,					// ���Q�b�gSE
	GetItemSE,					// �A�C�e���Q�b�gSE
	NetSE,						// ���Ԃ�U��SE
	TitleMSE,					// �^�C�g�����j���[�Z���N�gSE
	TitleMEnterSE,				// �^�C�g�����j���[����SE

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(void);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void PauseSound(SOUND_LABEL label);
#endif
