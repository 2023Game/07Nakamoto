#ifndef CMOVEWALL_H
#define CMOVEWALL_H

#include "CCharacter.h"
#include "CColliderMesh.h"
#include "CModel.h"
#include "CSwitch.h"

class CMoveWall : public CCharacter
{
public:

	//�R���X�g���N�^
	CMoveWall(CSwitch* parent, const CVector& position,
		const CVector& rotation, const CVector& scale);

	//�X�V����
	void Update();

	//�����ǂ̃��f���̎擾
	static CModel* GetModelWall();

	//�Փˏ���
	//void Collision();

private:
	//�X�C�b�`(�e)
	CSwitch* mpSwhitch;
	//���f�����i�[
	static CModel mModelWall;
	//�R���C�_
	CColliderMesh mColliderMesh;
	//CCollider mCollider;
	//�ړ�����
	float mMoveDistance;
	//�ړ���������
	float mMove;
};

#endif // !CMOVINGDOOR_H

