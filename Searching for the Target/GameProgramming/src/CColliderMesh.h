#ifndef CCOLLIDERMESH_H
#define CCOLLIDERMESH_H

#include "CModel.h"
#include "CColliderTriangle.h"
#include "CCollider.h"


/*
���f���f�[�^����O�p�R���C�_�̐���
*/
class CColliderMesh
{
	friend CCollider;
public:
	//�R���X�g���N�^
	CColliderMesh();
	//�f�X�g���N�^
	~CColliderMesh();

	/// <summary>
	/// ���f������O�p�R���C�_��ݒ�
	/// </summary>
	/// <param name="parent">�e</param>
	/// <param name="matrix">�e�s��</param>
	/// <param name="model">���f��</param>
	void ColliderMeshSet(CCharacter* parent, CMatrix* matrix, CModel* model
								, CCollider::ETag tag = CCollider::ETag::ENULL);

	static CModel* GetModelFloor();
	static CModel* GetModelObject();

private:
	//�O�p�R���C�_�̔z��쐬
	CColliderTriangle* mpColliderTriangles;

	static CModel mFloor;
	static CModel mObject;
};

#endif // !CCOLLIDERMESH_H
