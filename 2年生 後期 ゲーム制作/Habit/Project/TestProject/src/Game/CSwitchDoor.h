#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CPushSwitch;

// ���N���X
class CSwitchDoor : public CObjectBase
{
public:
	// �R���X�g���N�^
	CSwitchDoor(const CVector& pos, const CVector& angle, const CVector& size);
	// �f�X�g���N�^
	~CSwitchDoor();

	// �ڑ�����X�C�b�`��ǉ�
	void AddSwitch(CPushSwitch* sw);

	// �R���X�g���N�^�ŏ����Ă�����
	// ����������̍��W�ƊJ�������̍��W��ݒ肷��
	void SetAnimPos(const CVector& openPos, const CVector closePos);

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	// �X�C�b�`���S�ĉ�����Ă��邩�ǂ���
	bool IsSwitchOn() const;

	CModel* mpModel;	// ���̃��f���f�[�^
	CColliderMesh* mpColliderMesh;	// ���̃R���W�����f�[�^

	// ���̃h�A���J�����߂ɉ����K�v������X�C�b�`
	std::list<CPushSwitch*> mpSwitches;

	bool mIsOpened;	// �����J���Ă��邩�ǂ���

	CVector mClosePos;	// �����܂��Ă��鎞�̍��W
	CVector mOpenPos;	// �����J���Ă��鎞�̍��W
	float mAnimTime;	// �J����
	float mElapsedTime;	// �o�ߎ��ԕۑ��p
	bool mIsPlaying;	// �J�����ǂ���
};