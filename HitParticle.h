#pragma once

#include	<d3d11.h>
#include	<wrl/client.h>
#include	<DirectXMath.h>
#include	<random>
#include	"renderer.h"
#include	"CDirectInput.h"
#include	"dx11helper.h"
#include	"CMaterial.h"
#include	"CPlaneMesh.h"
#include	"CShader.h"
#include	"CMeshRenderer.h"

#include	"camera.h"
#include	"Player.h"

//�@�q�b�g�p�[�e�B�N����������
void InitHitParticle();
//�@�q�b�g�p�[�e�B�N���X�V����
void UpdateHitParticle();
//�@�q�b�g�p�[�e�B�N���`�揈��
void DrawHitParticle();
//�@�q�b�g�p�[�e�B�N���I������
void ExitHitParticle();

//�@�q�b�g�p�[�e�B�N���ݒ�
int SetHitParticle(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 move, DirectX::XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);

//�@�q�b�g�p�[�e�B�N��
void MakeHitParticle(DirectX::XMFLOAT3 pos);
//�@�G�l�~�[�L���p�[�e�B�N��
void MakeKillParticle(DirectX::XMFLOAT3 pos);
//�@�o���b�g�L���b�`�G�t�F�N�g
void MakeBulletCatchParticle(DirectX::XMFLOAT3 pos);


