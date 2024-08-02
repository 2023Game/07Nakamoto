#ifndef CMOVEFLOOR_H
#define CMOVEFLOOR_H

#include "CCharacter.h"
#include "CColliderMesh.h"
#include "CModel.h"
#include "CColliderLine.h"

class CMoveFloor :public CCharacter {

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�k</param>
	CMoveFloor(const CVector& position,
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
	//�܂�Ԃ�����
	float mTime;
	//�ړ�����
	CVector mMove;

	bool mFlag;

	//�R���C�_
	CColliderMesh mColliderMesh;
	//���R���C�_
	CColliderLine mCLine;

};

#endif // !CMOVEFLOOR_H

