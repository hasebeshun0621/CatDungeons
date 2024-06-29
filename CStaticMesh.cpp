#include	"CStaticMesh.h"
#include	"renderer.h"

using namespace DirectX;

	// ���f���t�@�C��������������
	bool CStaticMesh::Init(
		std::string filename)			//	�R�c���f���t�@�C��
	{
		bool sts{};

		m_filename = filename;			// �e�N�X�`���ǂݍ��ݗp�ɕۑ�

		// �V�[�����\�z
		Assimp::Importer importer;

		// �V�[�������\�z
		const aiScene* pScene = importer.ReadFile(
			filename.c_str(),
			aiProcess_ConvertToLeftHanded |
			// ������W�n�ɕϊ�����
//		aiProcess_PopulateArmatureData |
//		aiProcess_PreTransformVertices |
		// �m�[�h�\�����폜���ā@�����̃m�[�h�������[�J���ϊ��s��ł��ׂĂ̒��_�����O�ɕϊ�����
			aiProcess_Triangulate);
		// �O�p�`������
//		aiProcess_GenSmoothNormals |
//		aiProcess_JoinIdenticalVertices);

		if (pScene == nullptr) {

			printf("load error %s ( %s ) \n ", filename.c_str(), importer.GetErrorString());
			return false;
		}

		/*
			�ς�炸 �\������������
			const aiScene* pScene = aiImportFile(filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
			if (pScene == nullptr) {
				printf("load error %s ( %s ) \n ", filename.c_str(), importer.GetErrorString());
				return false;
			}
		*/

		// ���b�V�����Ń��b�V���R���e�i�z�񐔂�����
		m_subsets.resize(pScene->mNumMeshes);

		// ���b�V���������[�v
		for (unsigned int m = 0; m < pScene->mNumMeshes; m++) {

			aiMesh* mesh{};
			mesh = pScene->mMeshes[m];

			// ���b�V���P�ʂŒ��_�f�[�^���擾
			int vnum = GetVertices(mesh);

			// ���b�V���P�ʂŃC���f�b�N�X�f�[�^�擾
			int idxnum = GetIndices(mesh);

			// ���b�V���Ŏg�p���Ă���}�e���A�������擾
			int mtrlidx = mesh->mMaterialIndex;

			// �C���f�b�N�X����ۑ�
			m_subsets[m].IndexNum = idxnum;

			// ���_����ۑ�
			m_subsets[m].VertexNum = vnum;

			// �}�e���A���C���f�b�N�X��ۑ�
			m_subsets[m].MaterialIdx = mtrlidx;
		}

		// ���b�V���̃x�[�X�����\�z
		CalcMeshBaseIndex();

		// �S�e�N�X�`�����擾
		GetTextures(pScene);

		// �}�e���A�����擾
		GetMaterials(pScene);

		/*
		{
			FILE* fp;
			int i = 0;
			fopen_s(&fp, "vertex.txt", "w");
			for (auto& v : m_vertices) {
				fprintf(fp, "(%d) %f %f %f \n", i, v.Position.x, v.Position.y, v.Position.z);
				i++;
			}
			fclose(fp);
		}

		{
			FILE* fp;
			int i = 0;
			fopen_s(&fp, "index.txt", "w");
			for (auto& index : m_indices) {
				fprintf(fp, "(%d) %d \n", i, index);
				i++;
			}
			fclose(fp);
		}		
		*/

		return true;
	}

	void CStaticMesh::Exit() {

		// SRV���
	//	for (auto& srv : m_AllTexture) {
	//		srv->Release();
	//	}

		// SRV���
		for (auto& texres : m_Texturedic) {
			if (texres.second != nullptr) {
				texres.second->Release();
			}
		}
	}

	// �S���_��aiMesh����擾����
	unsigned int  CStaticMesh::GetVertices(const aiMesh* aimesh) {

		// ���_�������[�v
		for (unsigned int vidx = 0; vidx < aimesh->mNumVertices; vidx++) {

			VERTEX_3D v;

			v.Position.x = aimesh->mVertices[vidx].x;
			v.Position.y = aimesh->mVertices[vidx].y;
			v.Position.z = aimesh->mVertices[vidx].z;

			// �@������H
			if (aimesh->HasNormals()) {
				v.Normal.x = aimesh->mNormals[vidx].x;
				v.Normal.y = aimesh->mNormals[vidx].y;
				v.Normal.z = aimesh->mNormals[vidx].z;
			}
			else {
				v.Normal = XMFLOAT3(0, 0, 0);
			}

			// �e�N�X�`������H
			if (aimesh->HasTextureCoords(0)) {
				v.TexCoord.x = aimesh->mTextureCoords[0][vidx].x;				// 20230809 bug fix by ts
				v.TexCoord.y = aimesh->mTextureCoords[0][vidx].y;				// 20230809 bug fix by ts
			}
			else {
				v.TexCoord = XMFLOAT2(0, 0);
			}

			// ���_�J���[����H
			if (aimesh->HasVertexColors(0)) {
				v.Diffuse.x = aimesh->mColors[0][vidx].r;
				v.Diffuse.y = aimesh->mColors[0][vidx].g;
				v.Diffuse.z = aimesh->mColors[0][vidx].b;
				v.Diffuse.w = aimesh->mColors[0][vidx].a;
			}
			else {
				v.Diffuse = XMFLOAT4(1, 1, 1, 1);								// ���������ꍇ�͔�
			}

			// ���_�f�[�^��ۑ�
			m_vertices.emplace_back(v);
		}

		// aiMesh���ێ����钸�_����߂�
		return aimesh->mNumVertices;
	}

	// aiMesh����S�C���f�b�N�X�擾
	unsigned int  CStaticMesh::GetIndices(const aiMesh* aimesh) {

		// �ʐ������[�v
		for (unsigned int f = 0; f < aimesh->mNumFaces; f++)
		{
			const aiFace* face = &aimesh->mFaces[f];
			assert(face->mNumIndices == 3);

			// �C���f�b�N�X�f�[�^��ۑ�
			m_indices.emplace_back(face->mIndices[0]);
			m_indices.emplace_back(face->mIndices[1]);
			m_indices.emplace_back(face->mIndices[2]);

		}

		return aimesh->mNumFaces * 3;
	}

	// �x�[�X�����Čv�Z
	void CStaticMesh::CalcMeshBaseIndex() {

		// �T�u�Z�b�g�������[�v
		for (int m = 0; m < m_subsets.size(); m++)
		{
			// ���_�o�b�t�@�̃x�[�X���v�Z
			m_subsets[m].VertexBase = 0;
			for (int i = m - 1; i >= 0; i--) {
				m_subsets[m].VertexBase += m_subsets[i].VertexNum;
			}

			// �C���f�b�N�X�o�b�t�@�̃x�[�X���v�Z
			m_subsets[m].IndexBase = 0;
			for (int i = m - 1; i >= 0; i--) {
				m_subsets[m].IndexBase += m_subsets[i].IndexNum;
			}
		}
	}

	// ���ׂẴe�N�X�`��SRV���擾
	void CStaticMesh::GetTextures(const aiScene* aiscene) {

		ID3D11DeviceContext* devicecontext;
		ID3D11Device* device;

//		device = CDirectXGraphics::GetInstance()->GetDXDevice();
//		devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

		device = Renderer::GetDevice();
		devicecontext = Renderer::GetDeviceContext();

		// ���̃V�[�������e�N�X�`����S�ēǂݍ���
		for (unsigned int texidx = 0; texidx < aiscene->mNumTextures; texidx++) {

			// �e�N�X�`�����擾
			aiTexture* aitexture = aiscene->mTextures[texidx];

			// �����e�N�X�`�����H
			if (isEmbeddedTexure(aitexture)) {

				ID3D11ShaderResourceView* srv;

				aiTexture* aitexture = aiscene->mTextures[texidx];

				// mHeight is 0, so try to load a compressed texture of mWidth bytes
				const size_t size = aitexture->mWidth;

				// SRV����
				HRESULT hr = DirectX::CreateWICTextureFromMemory(
					device,
					devicecontext,
					reinterpret_cast<const unsigned char*>(aitexture->pcData),
					aitexture->mWidth,
					nullptr,
					&srv);

				assert(srv);

				// assimp�̓����f�[�^�ɃZ�b�g����Ă���e�N�X�`�������L�[�Ƃ��ĕۑ�
				m_Texturedic[aitexture->mFilename.data] = srv;

			}
			else {
				// �����e�N�X�`���ł͂Ȃ��ꍇ

				// �e�N�X�`���t�@�C�������擾����
				std::string filename = GetFileName(aitexture->mFilename.C_Str());

				// �e�N�X�`���t�@�C����ǂݍ���SRV�𐶐�
				// SRV
				ID3D11ShaderResourceView* srv = nullptr;
				//			srv = LoadDiffuseTexture(filename);

				bool sts = CreateSRVfromFile(
					filename.c_str(),
					device,
					devicecontext,
					&srv);

				if (sts) {
					// assimp�̓����f�[�^�ɃZ�b�g����Ă���e�N�X�`�������L�[�Ƃ��ĕۑ�
					m_Texturedic[aitexture->mFilename.data] = srv;
				}
				else {
					m_Texturedic[aitexture->mFilename.data] = nullptr;
				}
			}
		}
	}

	// �����e�N�X�`�����ǂ��������ʂ���
	bool CStaticMesh::isEmbeddedTexure(const aiTexture* aitexture) {

		if (aitexture->mHeight == 0 ||
			aitexture->mWidth == 0 ||
			aitexture->pcData == nullptr) {
			return true;
		}
		return false;
	}

	// �X�̃}�e���A�����i���ˏ��j�擾
	void CStaticMesh::GetaMatreial(const aiMaterial* aimtrl) {

		// �f�o�C�X�A�f�o�C�X�R���e�L�X�g�擾
		ID3D11DeviceContext* devicecontext;
		ID3D11Device* device;

//		device = CDirectXGraphics::GetInstance()->GetDXDevice();
//		devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();


		device = Renderer::GetDevice();
		devicecontext = Renderer::GetDeviceContext();

		// �}�e���A�����
		MATERIAL mymaterial{};

		// �g�U���ː���
		{
			aiColor3D color(0.0f, 0.0f, 0.0f);

			if (aimtrl->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
				mymaterial.Diffuse.x = color.r;
				mymaterial.Diffuse.y = color.g;
				mymaterial.Diffuse.z = color.b;
				mymaterial.Diffuse.w = 1.0f;
			}
			else {
				mymaterial.Diffuse.x = 0.5f;
				mymaterial.Diffuse.y = 0.5f;
				mymaterial.Diffuse.z = 0.5f;
				mymaterial.Diffuse.w = 1.0f;
			}
		}

		// ���ʔ��ː���
		{
			aiColor3D color(0.0f, 0.0f, 0.0f);

			if (aimtrl->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
				mymaterial.Specular.x = color.r;
				mymaterial.Specular.y = color.g;
				mymaterial.Specular.z = color.b;
				mymaterial.Specular.w = 1.0f;
			}
			else {
				mymaterial.Specular.x = 0.0f;
				mymaterial.Specular.y = 0.0f;
				mymaterial.Specular.z = 0.0f;
				mymaterial.Specular.w = 0.0f;
			}
		}

		// ���ʔ��ˋ��x
		{
			float Shinness = 0.0f;

			if (aimtrl->Get(AI_MATKEY_SHININESS, Shinness) == AI_SUCCESS) {
				mymaterial.Shininess = Shinness;
			}
			else {
				mymaterial.Shininess = 0.0f;
			}
		}

		// �������ː���
		{
			aiColor3D color(0.0f, 0.0f, 0.0f);

			if (aimtrl->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
				mymaterial.Ambient.x = color.r;
				mymaterial.Ambient.y = color.g;
				mymaterial.Ambient.z = color.b;
				mymaterial.Ambient.w = 1.0f;
			}
			else {
				mymaterial.Ambient.x = 0.0f;
				mymaterial.Ambient.y = 0.0f;
				mymaterial.Ambient.z = 0.0f;
				mymaterial.Ambient.w = 0.0f;
			}
		}

		// ���Ɣ�������
		{
			aiColor3D color(0.0f, 0.0f, 0.0f);

			if (aimtrl->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS) {
				mymaterial.Emission.x = color.r;
				mymaterial.Emission.y = color.g;
				mymaterial.Emission.z = color.b;
				mymaterial.Emission.w = 1.0f;
			}
			else {
				mymaterial.Emission.x = 0.0f;
				mymaterial.Emission.y = 0.0f;
				mymaterial.Emission.z = 0.0f;
				mymaterial.Emission.w = 0.0f;
			}
		}

		// �f�B�t���[�Y�e�N�X�`�����擾
		aiTextureType type = aiTextureType_DIFFUSE;
		int texnum = aimtrl->GetTextureCount(type);

		// 1���b�V���Ƀf�B�t�[�Y�e�N�X�`���P������������
		assert(texnum <= 1);

		// �}�e���A����
		aiString str;

		// assimp����e�N�X�`���t�@�C�������擾
		aimtrl->GetTexture(type, 0, &str);

		// SRV
		ID3D11ShaderResourceView* srv = nullptr;

		// SRV�����ɑ��݂��邩�H
		if (m_Texturedic.find(str.data) == m_Texturedic.end()) {
			mymaterial.TextureEnable = FALSE;
			srv = nullptr;

			// pmx �̏ꍇ�V�[���ɓo�^����Ă��Ȃ��e�N�X�`�������݂���
			aimtrl->GetTexture(type, 0, &str);

			// �t�@�C�������擾����
			std::string filename = GetFileName(str.C_Str());

			// �e�N�X�`����ǂݍ��ݎ擾
			// SRV
			srv = LoadDiffuseTexture(filename);

			if (srv == nullptr) {
				mymaterial.TextureEnable = FALSE;
			}
			else {
				mymaterial.TextureEnable = TRUE;
			}
		}
		else {
			mymaterial.TextureEnable = TRUE;
			srv = m_Texturedic[str.data];
		}

		// SRV�ۑ�
		m_AllTexture.emplace_back(srv);

		// �}�e���A������ۑ�
		m_materials.emplace_back(mymaterial);
	}

	// �}�e���A���擾
	void CStaticMesh::GetMaterials(const aiScene* aiscene) {

		// ���̃��f���Ŏg�p���Ă��邷�ׂẴ}�e���A�������擾
		for (unsigned int mrlidx = 0; mrlidx < aiscene->mNumMaterials; mrlidx++) {

			// ���̃��b�V���p�̃}�e���A�����擾
			aiMaterial* mtl;
			mtl = aiscene->mMaterials[mrlidx];

			// �}�e���A�����擾
			GetaMatreial(mtl);
		}
	}

// diffuse texture load
ID3D11ShaderResourceView* CStaticMesh::LoadDiffuseTexture(std::string filename) {
	// �t�@�C���̊g���q���擾
	std::string fileext = GetFileExt(filename);

	// �t�@�C����(UTF-16)���擾����
	std::wstring widefilename = GetFileNameWide(filename.c_str());

	// ���f���f�[�^�̐e�f�B���N�g�����擾
	std::wstring parentdirectory = GetParentDirectoryWide(m_filename);// +L"";

	// �e�N�X�`���̃p�X���擾
	std::wstring filepath = parentdirectory + widefilename;

	// SRV
	ID3D11ShaderResourceView* srv = nullptr;

	// TGA�H
	if (fileext == ".tga") {
		// TGA�̏ꍇ
		DirectX::TexMetadata meta;
		DirectX::GetMetadataFromTGAFile(filepath.c_str(), meta);

		std::unique_ptr<DirectX::ScratchImage> image(new DirectX::ScratchImage);

		HRESULT hr = LoadFromTGAFile(filepath.c_str(), &meta, *image);
		if (FAILED(hr)) {
			std::cout << "LoadFromTGAFile Error (" << filename << ")" << std::endl;
			return nullptr;
		}

		ID3D11Resource* tex = nullptr;

		// �e�N�X�`������
		hr = CreateTexture(
			Renderer::GetDevice(),
			image->GetImages(),
			image->GetImageCount(),
			meta,
			&tex);

		tex->Release();

		if (FAILED(hr)) {
			std::cout << "CreateTexture Error (" << filename << ")" << std::endl;
			return nullptr;
		}

		// �V�F�[�_�[���\�[�X�r���[�쐬
		hr = DirectX::CreateShaderResourceView(Renderer::GetDevice(),
			image->GetImages(),
			image->GetImageCount(),
			meta,
			&srv);

		if (FAILED(hr)) {
			std::wcout << L"CreateShaderResourceView Error (" << filepath.c_str() << L")" << std::endl;
			return nullptr;
		}
	}
	else {
		// �e�N�X�`���ǂݍ���
		HRESULT hr = DirectX::CreateWICTextureFromFile(
			Renderer::GetDevice(),
			filepath.c_str(),
			nullptr,
			&srv);
		if (FAILED(hr)) {
			std::wcout << L"CreateWICTextureFromFile Error (" << filepath.c_str() << widefilename.c_str() << L")" << std::endl;
			return nullptr;
		}
	}

	return srv;
}



