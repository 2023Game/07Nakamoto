#pragma once
#include "CInteractObject.h"
#include "CColliderMesh.h"

class CRDoor : public CInteractObject
{
public:
	// �R���X�g���N�^
	CRDoor(const CVector& pos, const CVector& angle, const CVector& openPos);
	// �f�X�g���N�^
	~CRDoor();

	// ���ׂ�
	void Interact() override;

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	CModel* mpR_Door;	// ���f���f�[�^
	CModel* mpR_DoorCol;// �R���C�_�[�f�[�^

	CColliderMesh* mpR_DoorColliderMesh;	// �R���C�_�[

	bool mIsOpened;	// �����J���Ă��邩�ǂ���

	CVector mClosePos;	// �����܂��Ă��鎞�̍��W
	CVector mOpenPos;	// �����J���Ă��鎞�̍��W
	float mAnimTime;	// �J����
	float mElapsedTime;	// �o�ߎ��ԕۑ��p
	bool mIsPlaying;	// �J�����ǂ���

};