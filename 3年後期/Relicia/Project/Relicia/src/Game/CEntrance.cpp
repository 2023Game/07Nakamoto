#include "CEntrance.h"

// �R���X�g���N�^
CEntrance::CEntrance(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Wall_Entrance");

	Position(pos);

}

// �f�X�g���N�^
CEntrance::~CEntrance()
{
}

// �X�V
void CEntrance::Update()
{
}

// �`��
void CEntrance::Render()
{
	mpModel->Render(Matrix());
}
