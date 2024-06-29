#include	"CatchPointer.h"

using namespace DirectX::SimpleMath;

//�@�|�C���^�\����
Pointer g_pointer;

// �V�F�[�_�[
static CShader		g_shader;

// �X�^�e�B�b�N���b�V���i�W�I���g���f�[�^�j
static CStaticMesh g_staticmesh[Model_Max];

// ���b�V�������_���[
static CStaticMeshRenderer g_staticmeshrenderer[Model_Max];

//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[//
//obb���b�V���A�V�F�[�_��`

// ���b�V�������_��
static CMeshRenderer g_meshrenderer;

// �}�e���A��
static CMaterial g_material;

// �{�b�N�X���b�V��
static CBoxMesh	g_box;

// OBB
static CBoundingBox g_obb;

//�@�|�C���^�}�g���b�N�X�擾
DirectX::SimpleMath::Matrix GetMtxPointer() {

	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_pointer.scale,
		g_pointer.rotation,
		g_pointer.position);

	return mtx;
}

// OBB�擾
BoundingBoxOBB GetPointerOBB() {
	return g_obb.MakeOBB(GetMtxPointer(), g_pointer.scale);
}

//�@�|�C���^�X�e�[�^�X���擾
int GetStsPointer() {
	return g_pointer.sts;
}

//�@�o���b�g�L���b�`���v���擾
int GetCatchCnt() {
	g_pointer.CatchCum = g_pointer.CatchBullet_Sura + g_pointer.CatchBullet_SuraG;
	return g_pointer.CatchCum;
}

//�@�^�C�v�ʃL���b�`���擾
int GetCatchBullet(int Type) {

	switch (Type)
	{
	case 0:
		return	999;
		break;
	case 1:
		return	g_pointer.CatchBullet_Sura;
		break;
	case 2:
		return	g_pointer.CatchBullet_SuraG;
		break;
	}

}

//�@�o���b�g���𑝂₷
void CatchCntUp(int Type) {

	switch (Type)
	{
	case 0:
		g_pointer.CatchBullet_Sura++;
		break;
	case 1:
		g_pointer.CatchBullet_SuraG++;
		break;
	}

}

//�@�o���b�g�������炷
void CatchCntDown(int Type) {

	switch (Type)
	{
	case 0:
		break;
	case 1:
		g_pointer.CatchBullet_Sura--;
		break;
	case 2:
		g_pointer.CatchBullet_SuraG--;
		break;
	}

}

//�@�|�C���^������
Pointer InitializPointer() {

	Pointer obj;
	obj.sts = 0;	//���-1�@�L���b�`�P

	obj.position = { 0,0,0 };			//�ʒu
	obj.rotation = { 0,0,0 };			//��]
	obj.scale = { 12,11,12 };			//�T�C�Y	
	obj.vector = { 0,0,0 };

	obj.CatchCum = 0;
	obj.CatchBullet_Sura = 0;
	obj.CatchBullet_SuraG = 0;

	obj.Catchfcnt = 0;
	obj.CatchReuse = 0;

	return obj;
}

//�@�|�C���^��������
void InitPointer() {

	// �g�p����V�F�[�_�[�𐶐�
	g_shader.SetShader(
		"shader/vertexLightingVS.hlsl",					// ���_�V�F�[�_
		"shader/vertexLightingPS.hlsl");				// �s�N�Z���V�F�[�_

	// ���f���t�@�C����
	std::string filename[] = {
		u8"assets\\model\\pointer.obj",
		u8"assets\\model\\net.obj"
	};

	//�|�C���^�ƃl�b�g�̓����
	for (int i = 0; i < Model_Max; i++) {
		// ���b�V�������i�W�I���g���f�[�^�j
		g_staticmesh[i].Init(filename[i]);

		// �`��ׂ̈̃f�[�^����
		g_staticmeshrenderer[i].Init(g_staticmesh[i]);


	}

	g_pointer = InitializPointer();

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[//

	// OBB����
	g_obb.Caliculate(g_staticmesh[1].GetVertices());

	// OBB�`��p���b�V������
	Vector3 diff = g_obb.GetMax() - g_obb.GetMin();
	g_box.Init(diff.x, diff.y, diff.z, Color(1, 1, 1, 1));
	g_meshrenderer.Init(g_box);

	// OBB�`��p�}�e���A��
	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 0.5f);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shininess = 0;
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.TextureEnable = FALSE;

	g_material.Init(mtrl);

}

//�@�|�C���^�X�V����
void UpdatePointer() {
	
	//�v���C���[�O���ʒu����Ɏ擾
	g_pointer.position = GetPositionPlayer();
	g_pointer.rotation = GetRotationPlayer();

	Matrix pmtx = GetMtxPlayer();
	g_pointer.position.x -= pmtx._31 * 0.8;
	g_pointer.position.y -= pmtx._32 -  4;
	g_pointer.position.z -= pmtx._33 * 0.8;
	
	//�@�X�y�[�X�������ƃl�b�g��U��
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
		if (g_pointer.CatchReuse <= 0) {
			g_pointer.CatchFlag = true;
			//�l�b�g�Ďg�p�\�܂ł̃^�C���ݒ�
			g_pointer.CatchReuse = 20;
			PlaySound(NetSE);
		}
	}
	
	if(g_pointer.CatchFlag&&g_pointer.Catchfcnt > 0){
		g_pointer.Catchfcnt--;
		g_pointer.sts = NET;
	}
	else {
		g_pointer.CatchFlag = false;
		//�l�b�g�W�J����
		g_pointer.Catchfcnt = 35;
		if (g_pointer.CatchReuse >= 0) {
			g_pointer.CatchReuse--;
		}
		g_pointer.sts = POINTER;
	}	
	
	MoveNet(g_pointer.CatchFlag);

}

//�@�|�C���^�`�揈��
void DrawPointer() {

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ���[���h�ϊ��s�񐶐�
	Matrix mtx;
	DX11MakeWorldMatrixRadian(
		mtx,
		g_pointer.scale,							// �X�P�[��
		g_pointer.rotation,							// �p��
		g_pointer.position);						// �ʒug_pointermtx

	// GPU�ɍs����Z�b�g����
	Renderer::SetWorldMatrix(&mtx);

	// �V�F�[�_�[��GPU�ɃZ�b�g
	g_shader.SetGPU();

	// ���f���`��
	g_staticmeshrenderer[g_pointer.sts].Draw();
	//-------------------------------------//���`��

	//// ���E�{�b�N�X�`��
	//mtx = g_obb.MakeWorldMtx(g_pointer.scale, mtx);

	//Renderer::SetWorldMatrix(&mtx);
	//g_material.SetGPU();
	//g_meshrenderer.Draw();

}

//�@�|�C���^�I������
void ExitPointer() {

	for (int i = 0; i < Model_Max; i++) {
		g_staticmeshrenderer[i].Uninit();
	}

}

//�@�l�b�g��U��
void MoveNet(bool flag) {

	static float rotcnt = 0;
	if (flag) {
		//�l�b�g��]���x
		rotcnt+=0.09f;
		g_pointer.rotation.y -= rotcnt;
		g_pointer.rotation.x -= rotcnt/3;
	}
	else {
		rotcnt = 0;
		
	}

}


