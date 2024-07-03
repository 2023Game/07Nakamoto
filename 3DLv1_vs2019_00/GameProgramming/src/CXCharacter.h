#ifndef CXCHARACTER_H
#define CXCHARACTER_H

#include "CModel.h"
#include "CMatrix.h"
#include "CCharacter3.h"

class CXCharacter :public CCharacter3{
public:
	//�f�t�H���g�R���X�g���N�^
	CXCharacter();
	//�f�X�g���N�^
	virtual ~CXCharacter() {
		SAFE_DELETE_ARRAY(mpCombinedMatrix);
	}
	//����������
	void Init(CModelX* model);
	//�A�j���[�V�����̕ύX
	void ChangeAnimation(int index, bool loop, float framesize);
	//�X�V����
	void Update();
	//�X�V����
	void Update(CMatrix& m);
	//�`�揈��
	void Render();
	//�A�j���[�V�����̍Đ��I������
	//true:�I�� false:�Đ���
	bool IsAnimationFinished();
	int AnimationIndex();	//�A�j���[�V�����ԍ��̎擾
protected:
	CMatrix* mpCombinedMatrix;	//�����s��ޔ�
	CModelX* mpModel;			//���f���f�[�^
	int mAnimationIndex;		//�A�j���[�V�����ԍ�
	bool mAnimationLoopFlg;		//true:�A�j���[�V�������J��Ԃ�
	float mAnimationFrame;		//�A�j���[�V�����Đ��t���[��
	float mAnimationFrameSize;	//�A�j���[�V�����̍Đ��t���[����
};
#endif // !CXCHARACTER_H
