#include "CTorch.h"

// �R���X�g���N�^
CTorch::CTorch(const CVector& pos, const CVector& angle, const CVector& size)
{
	// �ǂ̃��f���f�[�^�擾(�ύX�\��)
	mpModel = CResourceManager::Get<CModel>("Wall");

	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);
}

// �f�X�g���N�^
CTorch::~CTorch()
{
}

// �X�V����
void CTorch::Update()
{
}

// �`�揈��
void CTorch::Render()
{
	mpModel->Render(Matrix());
}
