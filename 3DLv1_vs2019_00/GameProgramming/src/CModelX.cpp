#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "CModelX.h"
#include "CMaterial.h"
#include "CVector.h"
#include "glut.h"

//�R���X�g���N�^
CMesh::CMesh()
	:mVertexNum(0)
	, mpVertex(nullptr)
	, mFaceNum(0)
	, mpVertexIndex(nullptr)
	, mNormalNum(0)
	, mpNormal(nullptr)
	, mMaterialNum(0)
	, mMaterialIndexNum(0)
	, mpMaterialIndex(nullptr)
{}

//�f�X�g���N�^
CMesh::~CMesh() {
	SAFE_DELETE_ARRAY(mpVertex);
	SAFE_DELETE_ARRAY(mpVertexIndex);
	SAFE_DELETE_ARRAY(mpNormal);
	SAFE_DELETE_ARRAY(mpMaterialIndex);
	//�X�L���E�F�C�g�̍폜
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		delete mSkinWeights[i];
	}
}

/*
Init
Mesh�̃f�[�^����荞��
*/
void CMesh::Init(CModelX* model) {
		model->GetToken();	// { or ���O
		if (!strchr(model->Token(), '{')) {
			//���O�̏ꍇ�A����{
			model->GetToken(); // {
		}

		//���_�̎擾
		mVertexNum = atoi(model->GetToken());
		//���_�����G���A�m��
		mpVertex = new CVector[mVertexNum];
		//���_�����f�[�^����荞��
		for (int i = 0; i < mVertexNum; i++) {
			mpVertex[i].X(atof(model->GetToken()));
			mpVertex[i].Y(atof(model->GetToken()));
			mpVertex[i].Z(atof(model->GetToken()));
		}

		//�ʐ��ǂݍ���
		mFaceNum = atoi(model->GetToken());
		//���_����1�ʂ�3���_
		mpVertexIndex = new int[mFaceNum * 3];
		for (int i = 0; i < mFaceNum * 3; i += 3) {
			model->GetToken();	//���_���ǂݔ�΂�
			mpVertexIndex[i] = atoi(model->GetToken());
			mpVertexIndex[i + 1] = atoi(model->GetToken());
			mpVertexIndex[i + 2] = atoi(model->GetToken());
		}

		//�P�ꂪ����ԌJ��Ԃ�
		while (!model->EOT()) {
			model->GetToken();		//MeshNormals
			// } �������̏ꍇ�͏I��
			if (strchr(model->Token(), '}'))
				break;
			if (strcmp(model->Token(), "MeshNormals") == 0) {
				model->GetToken();	// {
				//�@���f�[�^���擾
				mNormalNum = atoi(model->GetToken());
				//�@���̃f�[�^��z��Ɏ�荞��
				CVector* pNormal = new CVector[mNormalNum];
				for (int i = 0; i < mNormalNum; i++) {
					pNormal[i].X(atof(model->GetToken()));
					pNormal[i].Y(atof(model->GetToken()));
					pNormal[i].Z(atof(model->GetToken()));
				}
				//�@���� = �ʐ� �~ 3
				mNormalNum = atoi(model->GetToken()) * 3;	//FaceNum
				int ni;
				//���_���ɖ@���f�[�^��ݒ肷��
				mpNormal = new CVector[mNormalNum];
				for (int i = 0; i < mNormalNum; i += 3) {
					model->GetToken();	// 3
					ni = atoi(model->GetToken());
					mpNormal[i] = pNormal[ni];

					ni = atoi(model->GetToken());
					mpNormal[i + 1] = pNormal[ni];

					ni = atoi(model->GetToken());
					mpNormal[i + 2] = pNormal[ni];
				}
				delete[] pNormal;
				model->GetToken();	// }
			}	// End of MesfNormals
			//MeshMaterialList�̂Ƃ�
			else if (strcmp(model->Token(), "MeshMaterialList") == 0) {
				model->GetToken();	// {
				//Material�̐�
				mMaterialNum = atoi(model->GetToken());
				//FaceNum
				mMaterialIndexNum = atoi(model->GetToken());
				//�}�e���A���C���f�b�N�X�̍쐬
				mpMaterialIndex = new int[mMaterialIndexNum];
				for (int i = 0; i < mMaterialIndexNum; i++) {
					mpMaterialIndex[i] = atoi(model->GetToken());
				}
				//�}�e���A���f�[�^�̍쐬
				for (int i = 0; i < mMaterialNum; i++) {
					model->GetToken();	//Material
					if (strcmp(model->Token(), "Material") == 0) {
						mMaterial.push_back(new CMaterial(model));
					}
				}
				model->GetToken();	// } 
			}//End of MeshMaterialList
			//SkinWeights�̂Ƃ�
			else if (strcmp(model->Token(), "SkinWeights") == 0) {
				//CSkinWeights�N���X�̃C���X�^���X���쐬���A�z��ɒǉ�
				mSkinWeights.push_back(new CSkinWeights(model));
			}
			else
				//�ȊO�̃m�[�h�͓ǂݔ�΂�
				model->SkipNode();
		}

	//�f�o�b�O�o�[�W�����̂ݗL��
#ifdef _DEBUG

	printf("VertexNum:%d\n",mVertexNum);
	for (int i = 0; i < mVertexNum; i++)
	{
		printf("%10f %10f %10f\n",
			mpVertex[i].X(), mpVertex[i].Y(), mpVertex[i].Z());
	}

	printf("FaceNum:%d\n", mFaceNum);
	for (int i = 0; i < mFaceNum * 3; i += 3) {
		printf("%2d %2d %2d\n",
			mpVertexIndex[i], mpVertexIndex[i + 1], mpVertexIndex[i + 2]);
	}

	printf("NormalNum:%d\n", mNormalNum);
	for (int i = 0; i < mNormalNum; i++) {
		printf("%10f %10f %10f\n",
			mpNormal[i].X(), mpNormal[i].Y(), mpNormal[i].Z());
	}
#endif

}

/*
FindFrame(�t���[����)
�t���[�����ɊY������t���[���̃A�h���X��Ԃ�
*/
CModelXFrame* CModelX::FindFrame(char* name) {
	//�C�e���[�^�[�̍쐬
	std::vector<CModelXFrame*>::iterator itr;
	//�擪����Ō�܂ŌJ��Ԃ�
	for (itr = mFrame.begin(); itr != mFrame.end(); itr++) {
		//���O����v�������H
		if (strcmp(name, (*itr)->mpName) == 0) {
			//��v�����炻�̃A�h���X��Ԃ�
			return *itr;
		}
	}
	//��v����t���[�����Ȃ��ꍇ��nullptr��Ԃ�
	return nullptr;
}

/*
Render
��ʂɕ`�悷��
*/
void CMesh::Render()
{
	/*���_�f�[�^,�@���f�[�^�̔z���L���ɂ���*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	/*���_�f�[�^,�@���f�[�^�̏ꏊ���w�肷��*/
	glVertexPointer(3, GL_FLOAT, 0, mpVertex);
	glNormalPointer(GL_FLOAT, 0, mpNormal);

	/*���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷��*/
	for (int i = 0; i < mFaceNum; i++) {
		//�}�e���A����K�p����
		mMaterial[mpMaterialIndex[i]]->Enabled();
		glDrawElements(GL_TRIANGLES, 3,
			GL_UNSIGNED_INT, (mpVertexIndex + i * 3));
	}
	

	/*���_�f�[�^,�@���f�[�^�̔z��𖳌��ɂ���*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

char* CModelX::Token()
{
	return mToken;
}

CModelX::~CModelX()
{
	if (mFrame.size() > 0)
	{
		delete mFrame[0];
	}
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		delete mAnimationSet[i];
	}
}

bool CModelX::EOT()
{
	if (*mpPointer == '\0')
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
SkipNode
�m�[�h��ǂݔ�΂�
*/
void CModelX::SkipNode() {
	//�������I�������I��
	while (*mpPointer != '\0') {
		GetToken();	//���̒P��擾
		//{�����������烋�[�v�I��
		if (strchr(mToken, '{'))break;
	}
	int count = 1;

	//�������I��邩�A�J�E���g��0�ɂȂ�����I��
	while (*mpPointer != '\0' && count > 0) {
		GetToken();		//���̒P��擾
		//{��������ƃJ�E���g�A�b�v
		if (strchr(mToken, '{'))count++;
		//}�������ƃJ�E���g�_�E��
		else if (strchr(mToken, '}'))count--;
	}
}

/*
IsDelimiter(c)
c��\t\r\n �X�y�[�X�Ȃǂ̋󔒕���
�܂���,;"�Ȃǂ̕����ł����
��؂蕶���Ƃ���true��Ԃ�
*/
bool CModelX::IsDelimiter(char c)
{
	//isspace(c)
	//c���󔒕����Ȃ�0�ȊO��Ԃ�
	if (isspace(c) != 0)
		return true;

	/*
	strchr(������,����)
	������ɕ������܂܂�Ă���΁A
	�������������ւ̃|�C���^��Ԃ�
	������Ȃ�������NULL��Ԃ�
	*/
	if (strchr(",;\"", c) != NULL)
		return true;
	//��؂蕶���ł͂Ȃ�
	return false;
}

/*
GetToken
������f�[�^����A�P���1�擾����
*/
char* CModelX::GetToken() {
	char* p = mpPointer;
	char* q = mToken;
	//�^�u(\t)��()���s(\r)(\n), ; "�̋�؂蕶���ȊO�ɂȂ�܂œǂݔ�΂�
	while (*p != '\0' && IsDelimiter(*p))p++;
	if (*p == '{' || *p == '}') {
		//{�܂���}�Ȃ�mToken�ɑ�������̕�����
		*q++ = *p++;	//������ꂽ���1+�����
	}
	else {
		//�^�u(\t)��()���s(\r)(\n), ; " �̋�؂蕶���A
		//�܂��́A} �̕����ɂȂ�܂�mToken�ɑ������
		while (*p != '\0' && !IsDelimiter(*p) && *p != '}')
			*q++ = *p++;	//������ꂽ���+1�����
	}

	*q = '\0';		//mToken�̍Ō��\0����
	mpPointer = p;	//���̓ǂݍ��ރ|�C���g���X�V����

	//����mToken��//�̏ꍇ�́A�R�����g�Ȃ̂ŉ��s�܂œǂݔ�΂�
	/*
	strcmp(������1,������2)
	������1�ƕ�����2���������ꍇ�A0��Ԃ��܂��B
	������1�ƕ�����2���������Ȃ��ꍇ�A0�ȊO��Ԃ��܂�
	*/
	if (!strcmp("//", mToken)) {
		//���s�܂œǂݔ�΂�
		while (*p != '\0' && !strchr("\r\n", *p))p++;
		//�ǂݍ��݈ʒu�̍X�V
		mpPointer = p;
		//�P����擾����(�ċA�Ăяo��)
		return GetToken();
	}
	return mToken;
}

CModelX::CModelX()
	:mpPointer(nullptr)
{
	//mToken��������
	memset(mToken, 0, sizeof(mToken));

}

void CModelX::Load(char* file){
	//
	//�t�@�C���T�C�Y���擾����
	//
	FILE* fp;	//�t�@�C���|�C���^�ϐ��̍쐬
	fp = fopen(file, "rb");	//�t�@�C�����I�[�v������
	if (fp == NULL) {	//�G���[�`�F�b�N
		printf("fioen error:%s\n", file);
		return;
	}

	//�t�@�C���̍Ō�ֈړ�
	fseek(fp, 0L, SEEK_END);
	//�t�@�C���T�C�Y�̎擾
	int size = ftell(fp);
	//�t�@�C���T�C�Y+1�o�C�g���̗̈���m��
	char* buf = mpPointer = new char[size + 1];
	//
	//�t�@�C������3D���f���̃f�[�^��ǂݍ���
	//
	//�t�@�C���̐擪�ֈړ�
	fseek(fp, 0L, SEEK_SET);
	//�m�ۂ����̈�Ƀt�@�C���T�C�Y���f�[�^��ǂݍ���
	fread(buf, size, 1, fp);
	//�Ō�Ɂ_0��ݒ肷��(������̏I�[)
	buf[size] = '\0';

	//������̍Ō�܂ŌJ��Ԃ�
	while (*mpPointer != '\0'){
		GetToken();		//�P��̎擾
		//�P�ꂪFarmle�̏ꍇ
		if (strcmp(mToken, "Frame") == 0) {
			//�t���[�����쐬����
			new CModelXFrame(this);
		}
		//�P�ꂪAnimationSet�̏ꍇ
		else if (strcmp(mToken, "AnimationSet") == 0) {
			new CAnimationSet(this);
		}
	}

	fclose(fp);	//�t�@�C�����N���[�Y����

	SAFE_DELETE_ARRAY(buf);	//�m�ۂ����̈���������
}

/*
Render
�S�Ẵt���[���̕`�揈�����Ăяo��
*/
void CModelX::Render()
{
	for (size_t i = 0; i < mFrame.size(); i++) {
		mFrame[i]->Render();
	}
}

CModelXFrame::~CModelXFrame()
{
	//�q�t���[����S�ĉ������
	std::vector<CModelXFrame*>::iterator itr;
	for (itr = mChild.begin(); itr != mChild.end(); itr++) {
		delete* itr;
	}
	//���O�̃G���A���������
	SAFE_DELETE_ARRAY(mpName);

	if (mpMesh != nullptr)
	{
		delete mpMesh;
	}
}

/*
CModelXFrame
model:CModelX�C���X�^���X�ւ̃|�C���^
�t���[���𐧍삷��
�ǂݍ��ݒ���Frame��������΁A�t���[�����쐬��
�q�t���[���ɒǉ�����
*/
CModelXFrame::CModelXFrame(CModelX* model)
	:mpName(nullptr)
	,mIndex()
	,mpMesh(nullptr)
{
	//���݂̃t���[���z��̗v�f�����擾���ݒ肷��
	mIndex = model->mFrame.size();
	//CModelX�̃t���[���z��ɒǉ�����
	model->mFrame.push_back(this);
	//�ϊ��s���P�ʍs��ɂ���
	mTransformMatrix.Identity();
	//���̒P��(�t���[�����̗\��)���擾����
	model->GetToken();//frame name
	//�t���[�������G���A���m�ۂ���
	mpName = new char[strlen(model->mToken) + 1];
	//�t���[�������R�s�[����
	strcpy(mpName, model->mToken);
	//���̒P��({�̗\��)���擾����
	model->GetToken();	//{
	//�������Ȃ��Ȃ�����I���
	while (*model->mpPointer != '\0') {
		//���̒P��擾
		model->GetToken();	//Frame
		//}�������̏ꍇ�͏I��
		if (strchr(model->mToken, '}'))break;
		//�V�ȃt���[���̏ꏊ�́A�q�t���[���ɒǉ�
		if (strcmp(model->mToken, "Frame") == 0) {
			//�t���[�����쐬���A�q�t���[���̔z��ɒǉ�
			mChild.push_back(
				new CModelXFrame(model));
		}
		else if (strcmp(model->mToken, "FrameTransformMatrix") == 0) {
			model->GetToken(); // {
			for (int i = 0; i < mTransformMatrix.Size(); i++) {
				mTransformMatrix.M()[i] = atof(model->GetToken());
			}
			model->GetToken(); // }
		}
		else if (strcmp(model->mToken, "Mesh") == 0) {
			mpMesh = new CMesh();
			mpMesh->Init(model);
		}
		else {
			//��L�ȊO�̗v�f�͓ǂݔ�΂�
			model->SkipNode();
		}
	}
//�f�o�b�O�o�[�W�����̂ݗL��
#ifdef _DEBUG
	printf("%s\n", mpName);
	mTransformMatrix.Print();
#endif
}

int CModelXFrame::Index()
{
	return mIndex;
}

/*
Render
���b�V�������݂���Ε`�悷��
*/
void CModelXFrame::Render(){
	if (mpMesh != nullptr)
			mpMesh->Render();
}

/*
CSkinWeights
�X�L���E�F�C�g�̓ǂݍ���
*/
//�R���X�g���N�^
CSkinWeights::CSkinWeights(CModelX* model)
	:mpFrameName(nullptr)
	, mFrameIndex(0)
	, mIndexNum(0)
	, mpIndex(nullptr)
	, mpWeight(nullptr)
{
	model->GetToken();	// {
	model->GetToken();	// FrameName
	//�t���[�����G���A�m�ہA�ݒ�
	mpFrameName = new char[strlen(model->Token()) + 1];
	strcpy(mpFrameName, model->Token());

	//���_�ԍ��擾
	mIndexNum = atoi(model->GetToken());

	//���_�ԍ�����0�𒴂���
	if (mIndexNum > 0) {
		//���_�ԍ��ƒ��_�E�F�C�g�G���A�m��
		mpIndex = new int[mIndexNum];
		mpWeight = new float[mIndexNum];
		//���_�ԍ��擾
		for (int i = 0; i < mIndexNum; i++)
			mpIndex[i] = atoi(model->GetToken());
		//���_�E�F�C�g�擾
		for (int i = 0; i < mIndexNum; i++)
			mpWeight[i] = atof(model->GetToken());
	}
	//�I�t�Z�b�g�s��擾
	for (int i = 0; i < 16; i++) {
		mOffset.M()[i] = atof(model->GetToken());
	}
	model->GetToken();	// }

//�f�o�b�O�o�[�W�����̂ݗL��
#ifdef _DEBUG
	printf("SkinWeights %s\n",mpFrameName);
	for (int i = 0; i < mIndexNum; i++)
	{
		printf("%d %f\n", mpIndex[i], mpWeight[i]);
	}
	for (int i = 0; i < 16; i += 4)
	{
		printf("%f %f %f %f\n", mOffset.M()[i], mOffset.M()[i + 1], mOffset.M()[i + 2], mOffset.M()[i + 3]);
	}

#endif

}

//�f�X�g���N�^
CSkinWeights::~CSkinWeights()
{
	SAFE_DELETE_ARRAY(mpFrameName);
	SAFE_DELETE_ARRAY(mpIndex);
	SAFE_DELETE_ARRAY(mpWeight);
}

/*
CAnimationSet
*/
CAnimationSet::CAnimationSet(CModelX* model)
	:mpName(nullptr)
{
	model->mAnimationSet.push_back(this);
	model->GetToken();	//Ainmation Name
	//�A�j���[�V�����Z�b�g����ޔ�
	mpName = new char[strlen(model->Token()) + 1];
	strcpy(mpName, model->Token());
	model->GetToken();	// {
	while (!model->EOT()) {
		model->GetToken();	// } or Animation
		if (strchr(model->Token(), '}'))break;
		if (strcmp(model->Token(), "Animation") == 0) {
			//Animation�v�f�ǂݍ���
			mAnimation.push_back(new CAnimation(model));
		}
	}

//�f�o�b�O�o�[�W�����̂ݗL��
#ifdef _DEBUG
	printf("AnimationSet:%s\n", mpName);
#endif

}

//�f�X�g���N�^
CAnimationSet::~CAnimationSet()
{
	SAFE_DELETE_ARRAY(mpName);
	//�A�j���[�V�����v�f�̍폜
	for (size_t i = 0; i < mAnimation.size(); i++) {
		delete mAnimation[i];
	}
}

//�R���X�g���N�^
CAnimation::CAnimation(CModelX* model)
	: mpFrameName(nullptr)
	, mFrameIndex(0)
	, mKeyNum(0)
	, mpKey(nullptr)
{
	model->GetToken();	// { or Animation Name
	if (strchr(model->Token(), '{')) {
		model->GetToken();
	}
	else {
		model->GetToken();	// {
		model->GetToken();	// {
	}

	model->GetToken();	//FrameName
	mpFrameName = new char[strlen(model->Token()) + 1];
	strcpy(mpFrameName, model->Token());
	mFrameIndex =
		model->FindFrame(model->Token())->Index();
	model->GetToken();	// }
	//�L�[�̔z���ۑ����Ă����z��
	CMatrix* key[4] = { nullptr,nullptr,nullptr,nullptr };
	//���Ԃ̔z���ۑ����Ă����z��
	float* time[4] = { nullptr,nullptr,nullptr,nullptr };
	while (!model->EOT()) {
		model->GetToken();	// or AnimationKey
		if (strchr(model->Token(), '}'))break;
		if (strcmp(model->Token(), "AnimationKey") == 0) {
			model->GetToken(); // {
			//�f�[�^�̃^�C�v�擾
			int type = atoi(model->GetToken());
			//���Ԑ��擾
			mKeyNum = atoi(model->GetToken());
			switch (type) {
			case 0:	// Rotation Quaternion
				//�s��̔z������Ԑ����m��
				key[type] = new CMatrix[mKeyNum];
				//���Ԃ̔z������Ԑ����m��
				time[type] = new float[mKeyNum];
				//���Ԑ����J��Ԃ�
				for (int i = 0; i < mKeyNum; i++) {
					//���Ԏ擾
					time[type][i] = atof(model->GetToken());	
					model->GetToken();	//4��ǂݔ�΂�
					//w,x,y,z���擾
					float w = atof(model->GetToken());
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					//�N�H�[�^�j�I�������]�s��ɕϊ�
					key[type][i].Quaternion(x, y, z, w);
				}
				break;
			case 1: //�g��E�k���̍s��쐬
				key[type] = new CMatrix[mKeyNum];
				time[type] = new float[mKeyNum];
				for (int i = 0; i < mKeyNum; i++) {
					time[type][i] = atof(model->GetToken());
					model->GetToken(); // 3
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					key[type][i].Scale(x, y, z);
				}
				break;
			case 2: //�ړ��̍s��쐬
				key[type] = new CMatrix[mKeyNum];
				time[type] = new float[mKeyNum];
				for (int i = 0; i < mKeyNum; i++) {
					time[type][i] = atof(model->GetToken());
					model->GetToken(); // 3
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					key[type][i].Translate(x, y, z);
				}
				break;
			case 4: //�s��f�[�^�擾
				mpKey = new CAnimationKey[mKeyNum];
				for (int i = 0; i < mKeyNum; i++) {
					mpKey[i].mTime = atof(model->GetToken());	// Time
					model->GetToken(); // 16
					for (int j = 0; j < 16; j++) {
						mpKey[i].mMatrix.M()[j] = atof(model->GetToken());
					}
				}
				break;
			}
			model->GetToken(); // }
		}
		else {
			model->SkipNode();
		}
	}		//while�̏I���

	//�s��f�[�^�ł͂Ȃ��Ƃ�
	if (mpKey == nullptr) {
		//���Ԑ����L�[���쐬
		mpKey = new CAnimationKey[mKeyNum];
		for (int i = 0; i < mKeyNum; i++) {
			//���Ԑݒ�
			mpKey[i].mTime = time[2][i]; // Time
			//�s��쐬�@Scale * Rotation * Position
			mpKey[i].mMatrix = key[1][i] * key[0][i] * key[2][i];
		}
	}
	//�m�ۂ����G���A���
	for (int i = 0; i < ARRAY_SIZE(key); i++) {
		SAFE_DELETE_ARRAY(time[i]);
		SAFE_DELETE_ARRAY(key[i]);
	}

//�f�o�b�O�o�[�W�����̂ݗL��
#ifdef _DEBUG
	printf("Animation:%s\n",mpFrameName);
	for (int i = 0; i < 4; i++) {
		printf("%f %f %f %f\n", mpKey[i].mMatrix.M());
	}
	
#endif

}
//�f�X�g���N�^
CAnimation::~CAnimation() {
	SAFE_DELETE_ARRAY(mpFrameName);
	SAFE_DELETE_ARRAY(mpKey);
}