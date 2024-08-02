#ifndef COBJECT_H
#define COBJECT_H

#include "CCollider.h"
#include "CColliderMesh.h"
#include "CModel.h"

class CObject :public CCharacter
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�k</param>
	/// <param name="model">���f��</param>
	CObject(const CVector& pos,
		const CVector& rot, const CVector& scale, CModel* model);

	//��̃��f�����擾����
	static CModel* GetModelObject();
private:
	static CModel mModelObject;
	//�R���C�_
	CColliderMesh mCObject;
};

#endif // !COBJECT_H

