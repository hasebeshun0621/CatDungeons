#pragma once

#include	<DirectXMath.h>
#include	<d3d11.h>
#include	<wrl/client.h>
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

//�@�{�f�B�p�[�e�B�N����������
void InitBodyParticle();
//�@�{�f�B�p�[�e�B�N���X�V����
void UpdateBodyParticle();
//�@�{�f�B�p�[�e�B�N���`�揈��
void DrawBodyParticle();
//�@�{�f�B�p�[�e�B�N���I������
void ExitBodyParticle();

//�@�G�t�F�N�g�̐ݒ�
int SetBodyParticle(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 move, DirectX::XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);

//�@�񕜗p�p�[�e�B�N��
void MakeHealParticle(DirectX::XMFLOAT3 pos);
//�@�_���[�W�p�[�e�B�N��
void MakeDameParticle(DirectX::XMFLOAT3 pos);
//�@�e���˃p�[�e�B�N��
void MakeBulletFireParticle(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 col);
