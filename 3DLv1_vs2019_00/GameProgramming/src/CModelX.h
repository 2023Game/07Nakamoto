#ifndef CMODELX_H		//�C���N���[�h�K�[�h
#define CMODELX_H
#include <vector>		//vector�N���X�̃C���N���[�h(���I�z��)
#include "CMatrix.h"	//�}�g���N�X�N���X�̃C���N���[�h

class CMesh;			//CMesh�N���X�̐錾
class CModelX;			//CModelX�N���X�ɐ錾
class CModelXFrame;		//CModelXFeame�N���X�̐錾
class CMaterial;		//CMaterial�N���X�̐錾
class CSkinWeights;		//�X�L���E�F�C�g�N���X
class CAnimationSet;	//�A�j���[�V�����Z�b�g�N���X
class CAnimation;		//�A�j���[�V�����N���X
class CAnimationKey;	//�A�j���[�V�����L�[�N���X

#define MODEL_FILE "res\\���O�i.x"	//���̓t�@�C����

//�̈�J�����}�N����
#define SAFE_DELETE_ARRAY(a){if(a) delete[] a;a=nullptr;}

//�z��̃T�C�Y�擾���}�N����
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

class CMesh {
public:
	//�R���X�g���N�^
	CMesh();
	//�f�X�g���N�^
	~CMesh();

	void AnimateVertex(CMatrix*);

	//���_�ɃA�j���[�V�����K�p
	void AnimateVertex(CModelX* model);

	//�X�L���E�F�C�g�Ƀt���[���ԍ���ݒ肷��
	void SetSkinWeightFrameIndex(CModelX* model);

	//�ǂݍ��ݏ���
	void Init(CModelX* model);
	void Render();
private:
	//�e�N�X�`�����W�f�[�^
	float* mpTextureCoords;

	CVector* mpAnimateVertex;	//�A�j���[�V�����p���_
	CVector* mpAnimateNormal;	//�A�j���[�V�����p�@��

	//�X�L���E�F�C�g
	std::vector<CSkinWeights*> mSkinWeights;

	int mMaterialNum;	//�}�e���A����
	int mMaterialIndexNum;	//�}�e���A���ԍ���(�ʐ�)
	int* mpMaterialIndex;	//�}�e���A���ԍ�
	std::vector<CMaterial*> mMaterial;	//�}�e���A���f�[�^

	int mNormalNum;		//�@����
	CVector* mpNormal;	//�@���x�N�g��

	int mFaceNum;		//�ʐ�
	int* mpVertexIndex; //�ʂ��\�����钸�_�C���f�b�N�X

	int mVertexNum;		//���_��
	CVector* mpVertex;	//���_�f�[�^
};

/*
CModelX
X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
*/
class CModelX{
	friend CAnimationSet;
	friend CModelXFrame;
	friend CAnimation;
public:
	void AnimateVertex(CMatrix*);
	//�}�e���A���̌���
	CMaterial* FindMaterial(char* name);
	//�}�e���A���z��̎擾
	std::vector<CMaterial*>& Material();

	//���_�ɃA�j���[�V������K�p
	void AnimateVertex();
	//�X�L���E�F�C�g�̃t���[���ԍ��ݒ�
	void SetSkinWeightFrameIndex();
	//�t���[�����ɊY������t���[���̃A�h���X��Ԃ�
	CModelXFrame* FindFrame(char* name);
	//�g�[�N���������Ȃ�����true
	bool EOT();
	//�`��
	void Render();
	//mToken�̃|�C���^��Ԃ�
	char* Token();
	//�f�X�g���N�^
	~CModelX();
	//�m�[�h�̓ǂݔ�΂�
	void SkipNode();
	//�P��̎��o��
	char* GetToken();
	CModelX();
	//�t�@�C���ǂݍ���
	void Load(char* file);
	
	void AnimateFrame();

	std::vector<CModelXFrame*>& Frames();

	std::vector<CAnimationSet*> &AnimationSet();

private:
	//�}�e���A���z��
	std::vector<CMaterial*> mMaterial;
	//�A�j���[�V�����Z�b�g�̔z��
	std::vector<CAnimationSet*>mAnimationSet;

	std::vector<CModelXFrame*>mFrame;	//�t���[���̔z��
	//c����؂蕶���Ȃ�true��Ԃ�
	bool IsDelimiter(char c);
	char* mpPointer;		//�ǂݍ��݈ʒu
	char mToken[1024];	//���o�����P��̈�
};

//CModelXFrame�N���X�̒�`
class CModelXFrame {
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
public:
	const CMatrix& CombinedMatrix();

	//�����s��̍쐬
	void AnimateCombined(CMatrix* parent);
	//�f�t�H���g�R���X�g���N�^
	CModelXFrame();
	//�R���X�g���N�^
	CModelXFrame(CModelX* model);
	//�f�X�g���N�^
	~CModelXFrame();
	int Index();
	void Render();
private:
	CMatrix mCombinedMatrix;	//�����s��
	CMesh* mpMesh;	//Mesh�f�[�^
	std::vector<CModelXFrame*>mChild;	//�q�t���[���̔z��
	CMatrix mTransformMatrix;	//�ϊ��s��
	char* mpName;	//�t���[�����O
	int mIndex;		//�t���[���ԍ�
};

class CSkinWeights {
	friend CModelX;
	friend CMesh;
public:
	CSkinWeights(CModelX* model);
	~CSkinWeights();
	const int& FrameIndex();
	const CMatrix& Offset();
private:
	char* mpFrameName;	//�t���[����
	int mFrameIndex;	//�t���[���ԍ�
	int mIndexNum;		//���_�ԍ���
	int* mpIndex;		//���_�ԍ��z��
	float* mpWeight;	//���_�E�F�C�g�z��
	CMatrix mOffset;	//�I�t�Z�b�g�}�g���b�N�X
};

/*
CAnimationSet
�A�j���[�V�����Z�b�g
*/
class CAnimationSet {
	friend CModelX;
	
public:
	CAnimationSet(CModelX* model);
	~CAnimationSet();

	float Time();
	float MaxTime();

	void Time(float time);		//���Ԃ̐ݒ�
	void Weight(float weight);	//�d�݂̐ݒ�

	void AnimateMatrix(CModelX* model);
	std::vector < CAnimation*>& Animation();
private:
	float mTime;		//���ݎ���
	float mWeight;		//�d��
	float mMaxTime;		//�ő厞��

	//�A�j���[�V����
	std::vector<CAnimation*> mAnimation;
	//�A�j���[�V�����Z�b�g��
	char* mpName;
};

/*
CAnimation
�A�j���[�V�����N���X
*/
class CAnimation {
	friend CAnimationSet;
	friend CModelX;
public:
	CAnimation(CModelX* model);
	~CAnimation();
private:
	int mKeyNum;		//�L�[���i���Ԑ��j
	CAnimationKey* mpKey;	//�L�[�̔z��

	char* mpFrameName;	//�t���[����
	int mFrameIndex;	//�t���[���ԍ�

};

/*
CAnimarionKey
�A�j���[�V�����L�[�N���X
*/
class CAnimationKey {
	friend CAnimation;
	friend CAnimationSet;
private:
	//����
	float mTime;
	//�s��
	CMatrix mMatrix;
};

#endif