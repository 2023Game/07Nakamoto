#ifndef CFLOOR_H
#define CFLOOR_H

#include "CCollider.h"
#include "CColliderMesh.h"
#include "CModel.h"

class CFloor:public CCharacter
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�k</param>
	/// <param name="model">���f��</param>
	CFloor(const CVector& pos,
		const CVector& rot, const CVector& scale, CModel* model);

	//��̃��f�����擾����
	static CModel* GetModelFloor();
private:
	static CModel mModelFloor;
	//�R���C�_
	CColliderMesh mCFloor;
};

#endif // !CFLOOR_H

