#pragma once
#include "CInteractObject.h"
#include "CColliderMesh.h"

class CLDoor : public CInteractObject
{
public:
	// �R���X�g���N�^
	CLDoor(const CVector& pos,const CVector& rot);
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

	CColliderMesh* mpL_DoorColliderMesh;

	CVector mDefaultPos;
	CVector mMoveVec;
	float mElapsedTime;

	bool mOpen;	// �����J���Ă��邩
};