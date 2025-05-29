#pragma once
#include "CInteractObject.h"
#include "CColliderMesh.h"

class CLDoor : public CInteractObject
{
public:
	// �R���X�g���N�^
	CLDoor(const CVector& pos, const CVector& angle, const CVector& openPos);
	// �f�X�g���N�^
	~CLDoor();

	// ���ׂ�
	void Interact() override;

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	CModel* mpL_Door;	// ���f���f�[�^
	CModel* mpL_DoorCol;// �R���C�_�[�f�[�^

	CColliderMesh* mpL_DoorColliderMesh;	// �R���C�_�[

	bool mIsOpened;	// �����J���Ă��邩�ǂ���
	bool mIsBroken;	// ������ꂽ�ǂ���

	CVector mClosePos;	// �����܂��Ă��鎞�̍��W
	CVector mOpenPos;	// �����J���Ă��鎞�̍��W
	float mAnimTime;	// �J����
	float mElapsedTime;	// �o�ߎ��ԕۑ��p
	bool mIsPlaying;	// �J�����ǂ���
};