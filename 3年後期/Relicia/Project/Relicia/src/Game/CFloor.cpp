#include "CFloor.h"

// �R���X�g���N�^
CFloor::CFloor(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Floor"),

	Position(pos);
}

// �f�X�g���N�^
CFloor::~CFloor()
{
}

// �X�V
void CFloor::Update()
{
}

// �`��
void CFloor::Render()
{
	mpModel->Render(Matrix());
}
