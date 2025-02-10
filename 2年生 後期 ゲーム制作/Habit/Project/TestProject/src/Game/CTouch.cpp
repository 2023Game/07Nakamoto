#include "CTouch.h"

// �R���X�g���N�^
CTouch::CTouch(const CVector& pos, const CVector& angle, const CVector& size)
{
	// �ǂ̃��f���f�[�^�擾(�ύX�\��)
	//mpModel = CResourceManager::Get<CModel>("Obj");
	mpModel = CResourceManager::Get<CModel>("Tourou");

	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);
}

// �f�X�g���N�^
CTouch::~CTouch()
{
}

// �X�V����
void CTouch::Update()
{
}

// �`�揈��
void CTouch::Render()
{
	mpModel->Render(Matrix());
}
