#include "CStand.h"
#include "CColliderMesh.h"

//�R���X�g���N�^
CStand::CStand(const CVector& pos)
	: CObjectBase(ETag::eField)
{
	mpStandModel = CResourceManager::Get <CModel>("Stand");
	CModel* stand_col = CResourceManager::Get <CModel>("StandCol");

	Position(pos);
	mpStandColMesh = new CColliderMesh(this, ELayer::eField, stand_col, true);



}

// �f�X�g���N�^
CStand::~CStand()
{
	if (mpStandColMesh != nullptr)
	{
		SAFE_DELETE(mpStandColMesh);

	}
}

// �`��
void CStand::Render()
{
	mpStandModel->Render(Matrix());
}
