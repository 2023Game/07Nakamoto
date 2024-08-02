#ifndef SLOPE_H
#define SLOPE_H

#include "CCharacter.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CSlope : public CCharacter
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�k</param>
	/// <param name="model">���f��</param>
	CSlope(const CVector& pos,
		const CVector& rot, const CVector& scale,CModel *model);

	//�Փˏ���
	//void Collision();

	CMatrix* GetMatrix();

	void Update();

	//��̃��f�����擾����
	static CModel* GetModelSlope();

private:
	static CModel mModelSlope;

	//�R���C�_
	CColliderMesh mCSlope;

};

#endif // !SLOPE_H
