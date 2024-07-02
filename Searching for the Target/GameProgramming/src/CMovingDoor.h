#ifndef CMOVINGDOOR_H
#define CMOVINGDOOR_H

#include "CCharacter.h"
#include "CColliderMesh.h"
#include "CModel.h"
#include "CSwitch.h"

class CMovingDoor : public CCharacter
{
public:

	//�f�t�H���g�R���X�g���N�^
	CMovingDoor();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="parent">�e�N���X</param>
	/// <param name="position">�ʒu</param>
	/// <param name="rotation">��]</param>
	/// <param name="scale">�g�k</param>
	CMovingDoor(CSwitch* parent, const CVector& position,
		const CVector& rotation, const CVector& scale);

	void SetMovingDoor(CSwitch* parent, const CVector& position,
		const CVector& rotation, const CVector& scale);

	//�X�V����
	void Update();

	//���f���̎擾
	static CModel* GetModelCube();

private:
	//�X�C�b�`(�e)
	CSwitch* mpSwhitch;
	//���f�����i�[
	static CModel mModelCube;
	//�R���C�_
	CColliderMesh mColliderMesh;
	//�ړ�����
	float mMoveDistance;
	//�ړ���������
	float mMove;
};

#endif // !CMOVINGDOOR_H

