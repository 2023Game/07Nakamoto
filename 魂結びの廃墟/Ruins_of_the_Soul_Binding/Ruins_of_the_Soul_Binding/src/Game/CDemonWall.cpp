#include "CDemonWall.h"
#include "CColliderMesh.h"
#include "CNavManager.h"

// �R���X�g���N�^
CDemonWall::CDemonWall()
	: CObjectBase(ETag::eField)
	, mpModel(nullptr)
	, mpColliderMesh(nullptr)
{
	mpModel = CResourceManager::Get<CModel>("DemonWall");

	CModel* col = CResourceManager::Get<CModel>("DemonWallCol");
	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, col, true);

	// �o�H�T���p�̎Օ����`�F�b�N�̃R���C�_�[�ɁA�d�͂̕ǂ̃R���C�_�[��o�^
	CNavManager::Instance()->AddCollider(mpColliderMesh);
}

// �f�X�g���N�^
CDemonWall::~CDemonWall()
{
	SAFE_DELETE(mpColliderMesh);
}

// �`��
void CDemonWall::Render()
{
	mpModel->Render(Matrix());
}
