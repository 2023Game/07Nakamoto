#ifndef CMOVEFLOOR_H
#define CMOVEFLOOR_H

#include "CCharacter.h"
#include "CColliderMesh.h"
#include "CModel.h"
#include "CColliderLine.h"

class CMoveFloor :public CCharacter {

public:
	//�f�t�H���g�R���X�g���N�^
	CMoveFloor();

	/// <summary>
	/// �������̐ݒ�
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�k</param>
	void SetMoveFloor(const CVector& position,
		const CVector& rotation, const CVector& scale);

	//�������̃��f���̎擾
	static CModel* GetModelFloor();
	
	//�X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="m">�����̃R���C�_</param>
	/// <param name="o">����̃R���C�_</param>
	void Collision(CCollider* m, CCollider* o);

	//�Փˏ���
	void Collision();

private:
	//���f�����i�[
	static CModel mModelFloor;
	//�ړ�����
	float mMoveDistance;
	//�ړ���������
	float mMove;

	//�R���C�_
	CColliderMesh mColliderMesh;
	//���R���C�_
	CColliderLine mCLine;

};

#endif // !CMOVEFLOOR_H

