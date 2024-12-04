#include "CPushSwitch.h"
#include "CPushSwitchManager.h"
#include "Primitive.h"

// �R���X�g���N�^
CPushSwitch::CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size)
	: mSwitch(false)
	, mNum(1)
{
	// �Ǘ��N���X�̃��X�g�Ɏ��g��ǉ�
	CPushSwitchManager* pushMgr = CPushSwitchManager::Instance();
	if (pushMgr != nullptr)
	{
		pushMgr->AddSwitch(this);
	}

	// �X�C�b�`�̃��f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("Switch");

	// �X�C�b�`�̃R���C�_�[���擾
	CModel* colModel = CResourceManager::Get<CModel>("Switch");
	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);

	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);

	// �Ǘ��N���X�̃��X�g�̐���ԍ��ɂ��悤����
	mNumber = mNum;
}

// �f�X�g���N�^
CPushSwitch::~CPushSwitch()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpColliderMesh);
}

// TODO�F�v���C���[���X�C�b�`��������͈͂ɓ����Ă��邩�ǂ���
//bool CPushSwitch::IsRangePlayer() const
//{
//	return false;
//}

// �X�C�b�`�̏�Ԃ��I�����I�t��
bool CPushSwitch::IsOnSwtch()
{
	return mSwitch;
}

// �ԍ����擾
int CPushSwitch::GetNumber()
{
	return mNumber;
}

// �`�揈��
void CPushSwitch::Render()
{
	mpModel->Render(Matrix());
}
