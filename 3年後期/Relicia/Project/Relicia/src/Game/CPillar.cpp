#include "CPillar.h"

// �R���X�g���N�^
CPillar::CPillar(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Pillar"),

	Position(pos);

}

// �f�X�g���N�^
CPillar::~CPillar()
{
}

// �X�V
void CPillar::Update()
{
}

// �`��
void CPillar::Render()
{
	mpModel->Render(Matrix());
}
