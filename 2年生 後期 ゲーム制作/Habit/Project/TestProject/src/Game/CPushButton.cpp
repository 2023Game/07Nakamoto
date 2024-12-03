#include "CPushButton.h"
#include "CColliderTriangle.h"
#include "CPlayer2.h"
#include "Primitive.h"

// �v���C���[�̃C���X�^���X
CPushButton* CPushButton::spInstance = nullptr;

// �R���X�g���N�^
CPushButton::CPushButton(const CVector& pos, const CVector& angle, const CVector& size)
	: mPush(false)
{
	// �C���X�^���X�̐ݒ�
	spInstance = this;

	// �{�^���̃��f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("Wall");

	// �{�^���̃R���C�_�[���擾
	CModel* colModel = CResourceManager::Get<CModel>("WallCol");
	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);

	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);
}

// �f�X�g���N�^
CPushButton::~CPushButton()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpColliderMesh);
}

bool CPushButton::isRangePlayer() const
{
	return false;
}

// �C���X�^���X�̎擾
CPushButton* CPushButton::Instance()
{
	// �v���C���[�����݂��Ȃ��ꍇ
	CPlayer2* player = CPlayer2::Instance();
	if (player == nullptr)return false;

	//�v���C���[�̍��W���擾
	CVector playerPos = player->Position();
	// �������g�̍��W���擾
	//CVector pos = Position();
	// ���g����v���C���[�܂ł̃x�N�g�������߂�
	//CVector  vec = playerPos - pos;
	//vec.Y(0.0f);	//�v���C���[�Ƃ̍����̍����l�����Ȃ�

	return spInstance;
}

// �X�V����
void CPushButton::Update()
{
}

// �`�揈��
void CPushButton::Render()
{
	mpModel->Render(Matrix());
}
