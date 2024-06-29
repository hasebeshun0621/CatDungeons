#pragma once

#include	<DirectXMath.h>
#include	<d3d11.h>
#include	<wrl/client.h>
#include	<random>

#include	"renderer.h"
#include	"CDirectInput.h"
#include	"dx11helper.h"
#include	"camera.h"
#include	"CMaterial.h"
#include	"CPlaneMesh.h"
#include	"CShader.h"
#include	"CMeshRenderer.h"
#include	"Player.h"

//�@�_�b�V���p�[�e�B�N����������
bool InitDashParticle(void);
//�@�_�b�V���p�[�e�B�N���X�V����
void UpdateDashParticle(void);
//�@�_�b�V���p�[�e�B�N���`�揈��
void DrawDashParticle(void);
//�@�_�b�V���p�[�e�B�N���I������
void ExitDashParticle(void);
//�@�_�b�V���p�[�e�B�N���ݒ�
int SetDashParticle(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 move, DirectX::XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);
//�@�_�b�V���p�[�e�B�N��
void MakeDashParticle(DirectX::XMFLOAT3 pos);