#include "CItemObj.h"
#include "CColliderSphere.h"
#include "CInventory.h"

// �R���X�g���N�^
CItemObj::CItemObj(ItemType type)
	: CInteractObject(ETaskPriority::eItem,0,ETaskPauseType::eGame)
	, mItemTyope(type)
	, mpItemData(nullptr)
	, mpModel(nullptr)
	, mpCollider(nullptr)
{
	// �w�肳�ꂽ�A�C�e���̎�ނ���A�C�e���f�[�^���擾
	bool success = Item::GetItemData(mItemTyope, &mpItemData);
	// �A�C�e���f�[�^�����݂��Ȃ�������A���g���폜
	if (!success)
	{
		Kill();
		return;
	}

	// �A�C�e���f�[�^�̃��f���f�[�^���擾
	mpModel = CResourceManager::Get<CModel>(mpItemData->modelPath);

	// �R���C�_�[���쐬
	CreateCollider();

	// ���ׂ�Ƃ��ɕ\������e�L�X�g��ݒ�
	mInteractStr = "�E��";
}

// �f�X�g���N�^
CItemObj::~CItemObj()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpCollider);
}

// �R���C�_�[�쐬(�p����ŏ㏑����)
void CItemObj::CreateCollider()
{
	// �R���C�_�[���쐬
	mpCollider = new CColliderSphere
	(
		this, ELayer::eInteractObj,
		10.0f, true
	);

	// �v���C���[�ƏՓ˂���悤�ɐݒ�
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::eInteractSearch });

}

// ���ׂ����Ԃ��ǂ���
bool CItemObj::CanInteract() const
{
	if (IsKill()) return false;

	return true;
}

// ���ׂ�
void CItemObj::Interact()
{
	//�C���x���g���ɒǉ�
	CInventory::Instance()->AddItem(mItemTyope, 1);

	Kill();

}

// �X�V����
void CItemObj::Update()
{
}

// �`�揈��
void CItemObj::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}

