#include "CDoor.h"

// �R���X�g���N�^
CDoor::CDoor(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Door"),

	Position(pos);

}

// �f�X�g���N�^
CDoor::~CDoor()
{

}

// �X�V
void CDoor::Update()
{
}

// �`��
void CDoor::Render()
{
	mpModel->Render(Matrix());
}
