#include "CColliderMesh.h"
#include "CCollider.h"

//�f�t�H���g�R���X�g���N�^
CColliderMesh::CColliderMesh()
	:mpColliderTriangles(nullptr)
{

}

//�f�X�g���N�^
CColliderMesh::~CColliderMesh()
{
	if (mpColliderTriangles != nullptr)
	{
		//�O�p�R���C�_�z��̍폜
		delete[] mpColliderTriangles;
	}
}

//���f������O�p�R���C�_�̐���
void CColliderMesh::ColliderMeshSet(CCharacter* parent, CMatrix* matrix, CModel* model,CCollider::ETag tag)
{
	//���f���̎O�p�|���S���ŎO�p�R���C�_�̔z��쐬
	mpColliderTriangles = new
		CColliderTriangle[model->Triangles().size()];
	for (size_t i = 0; i < model->Triangles().size(); i++)
	{
		//�^�O�̐ݒ�
		mpColliderTriangles[i].SetTag(tag);
		//�O�p�R���C�_�̐ݒ�
		mpColliderTriangles[i].SetColliderTriangle(parent, matrix
			, model->Triangles()[i].GetV0()
			, model->Triangles()[i].GetV1()
			, model->Triangles()[i].GetV2());

		//�D��x�̕ύX
		mpColliderTriangles[i].ChangePriority();
	}
}

//static�ϐ��̒�`
CModel CColliderMesh::mFloor;
//���̃��f�����擾����
CModel* CColliderMesh::GetModelFloor()
{
	return &mFloor;
}
//static�ϐ��̒�`
CModel CColliderMesh::mObject;
//�I�u�W�F�N�g�̃��f�����擾����
CModel* CColliderMesh::GetModelObject()
{
	return &mObject;
}