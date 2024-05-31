#include "CColliderMesh.h"

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
void CColliderMesh::ColliderMeshSet(CCharacter* parent, CMatrix* matrix, CModel* model)
{
	//���f���̎O�p�|���S���ŎO�p�R���C�_�̔z��쐬
	mpColliderTriangles = new
		CColliderTriangle[model->Triangles().size()];
	for (size_t i = 0; i < model->Triangles().size(); i++)
	{
		//�O�p�R���C�_�̐ݒ�
		mpColliderTriangles[i].SetColliderTriangle(parent, matrix
			, model->Triangles()[i].GetV0()
			, model->Triangles()[i].GetV1()
			, model->Triangles()[i].GetV2());
		mpColliderTriangles[i].ChangePriority();
	}
}

//�D��x�̕ύX