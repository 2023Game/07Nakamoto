#include "CStand.h"
#include "CColliderMesh.h"
#include "CNavManager.h"

//�R���X�g���N�^
CStand::CStand(const CVector& pos)
	: CObjectBase(ETag::eField)
	, mpStandWallColMesh(nullptr)
	, mpStandFloorColMesh(nullptr)
{
	mpStandModel = CResourceManager::Get <CModel>("Stand");

	Position(pos);

	// �y��̕Ǖ����̃R���C�_�[�쐬
	CModel* standWallCol = CResourceManager::Get <CModel>("StandWallCol");
	mpStandWallColMesh = new CColliderMesh(this, ELayer::eWall, standWallCol, true);
	// �y��̏������̃R���C�_�[�쐬
	CModel* standFloorCol = CResourceManager::Get <CModel>("StandFloorCol");
	mpStandFloorColMesh = new CColliderMesh(this, ELayer::eFloor, standFloorCol, true);

	// �o�H�T���p�̎Օ����`�F�b�N�̃R���C�_�[�ɁA���̃R���C�_�[��o�^
	CNavManager::Instance()->AddCollider(mpStandWallColMesh);
	CNavManager::Instance()->AddCollider(mpStandFloorColMesh);

}

// �f�X�g���N�^
CStand::~CStand()
{
	SAFE_DELETE(mpStandWallColMesh);
	SAFE_DELETE(mpStandFloorColMesh);
}

// �`��
void CStand::Render()
{
	mpStandModel->Render(Matrix());
}
