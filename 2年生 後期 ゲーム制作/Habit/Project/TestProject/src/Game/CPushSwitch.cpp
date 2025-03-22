#include "CPushSwitch.h"
#include "Primitive.h"
#include "CPlayer2.h"
#include "Maths.h"
#include "CColliderSphere.h"
#include "CBillBoard.h"
#include "CInteractUI.h"

#define POSITION CVector(0.0f,15.0f,0.0f)

// �R���X�g���N�^
CPushSwitch::CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size)
	: mSwitch(false)
{
	// �X�C�b�`�̃��f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("Switch");

	// �X�C�b�`�̃R���C�_�[����
	mpCollider = new CColliderSphere
	(
		this, ELayer::eInteractObj,
		4.0f, true
	);
	mpCollider->Position(0.0f, 2.0f, 0.0f);
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer, ELayer::eInteractSearch });

	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);

	mInteractStr = "�I���ɂ���";

	// �C���^���N�g�{�^���̕\��
	mpInteractUI = new CInteractUI(this);
	mpInteractUI->Position(Position() + POSITION);

}

// �f�X�g���N�^
CPushSwitch::~CPushSwitch()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpCollider);
	SAFE_DELETE(mpInteractUI);
}

// �X�C�b�`�̏�Ԃ��I�����I�t��
bool CPushSwitch::IsOnSwtch()
{
	return mSwitch;
}

// ���ׂ�
void CPushSwitch::Interact()
{
	mSwitch = !mSwitch;
	mInteractStr = mSwitch ? "�I�t�ɂ���" : "�I���ɂ���";
}

// �`�揈��
void CPushSwitch::Render()
{
	if (mSwitch)
	{
		// �X�C�b�`�̃��f���f�[�^�擾
		mpModel = CResourceManager::Get<CModel>("Switch_ON");
	}
	else
	{
		// �X�C�b�`�̃��f���f�[�^�擾
		mpModel = CResourceManager::Get<CModel>("Switch_OFF");
	}

	mpInteractUI->Render();
	mpModel->Render(Matrix());
}
