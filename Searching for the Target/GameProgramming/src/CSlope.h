#ifndef SLOPE_H
#define SLOPE_H

#include "CCharacter.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CSlope : public CCharacter
{
public:

	//�f�t�H���g�R���X�g���N�^
	CSlope();

	/// <summary>
	/// ���f���̐ݒ�
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�k</param>
	/// <param name="model">���f��</param>
	void SetSlope(const CVector& pos,
		const CVector& rot, const CVector& scale,CModel *model);

	//�Փˏ���
	void Collision();

	CMatrix* GetMatrix();

	//��̃��f�����擾����
	static CModel* GetModelSlope();

private:
	static CModel mModelSlope;

	//�R���C�_
	CCollider mColliderSlope;

};

#endif // !SLOPE_H
