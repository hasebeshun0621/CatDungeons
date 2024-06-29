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


//�@�L���b�`�p�[�e�B�N����������
void InitCatchParticle();
//�@�L���b�`�p�[�e�B�N���X�V����
void UpdateCatchParticle();
//�@�L���b�`�p�[�e�B�N���`�揈��
void DrawCatchParticle();
//�@�L���b�`�p�[�e�B�N���I������
void ExitCatchParticle();

//�@�L���b�`�G�t�F�N�g�̐ݒ�
int SetCatchParticle(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 move, DirectX::XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);

//�@�L���b�`�G�t�F�N�g
void MakeCatchParticle(DirectX::XMFLOAT3 pos);