#include "CWall.h"

// �R���X�g���N�^
CWall::CWall(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Wall"),

	Position(pos);
}

// �f�X�g���N�^
CWall::~CWall()
{
}

// �X�V
void CWall::Update()
{
}

// �`��
void CWall::Render()
{
	mpModel->Render(Matrix());
}
