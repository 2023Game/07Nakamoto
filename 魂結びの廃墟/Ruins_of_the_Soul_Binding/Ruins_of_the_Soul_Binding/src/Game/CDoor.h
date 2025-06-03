#pragma once
#include "CInteractObject.h"
#include "CColliderMesh.h"

class CDoor : public CInteractObject
{
public:
	// �R���X�g���N�^
	CDoor(const CVector& pos, const CVector& angle, const CVector& openPos, std::string modelName, std::string colName);
	// �f�X�g���N�^
	virtual ~CDoor();

	// ���ׂ�
	void Interact() override;
	// ��ꂽ���̏���
	void Death();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	CModel* mpDoor;	// ���f���f�[�^
	CModel* mpDoorCol;// �R���C�_�[�f�[�^

	CColliderMesh* mpDoorColliderMesh;	// �R���C�_�[

	bool mIsOpened;	// �����J���Ă��邩�ǂ���

	CVector mClosePos;	// �����܂��Ă��鎞�̍��W
	CVector mOpenPos;	// �����J���Ă��鎞�̍��W
	float mAnimTime;	// �J����
	float mElapsedTime;	// �o�ߎ��ԕۑ��p
	bool mIsPlaying;	// �J�����ǂ���
};