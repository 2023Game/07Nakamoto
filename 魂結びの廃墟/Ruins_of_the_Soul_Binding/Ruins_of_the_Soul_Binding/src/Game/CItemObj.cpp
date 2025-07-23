#include "CItemObj.h"
#include "CColliderSphere.h"
#include "CInventory.h"
#include "CImage3D.h"

#define INTERACT_TEXT_PATH "UI\\Interact\\pickup.png"
#define WORLD_UNIT_PER_PIXEL 100.0f
#define MIN_SIZ		CVector2(50.0f,50.0f)

// �R���X�g���N�^
CItemObj::CItemObj(ItemType type)
	: CInteractObject(ETaskPriority::eItem, 0, ETaskPauseType::eGame)
	, mItemTyope(type)
	, mpItemData(nullptr)
	, mpItemImage(nullptr)
	, mpCollider(nullptr)
{
	// ���ׂ�e�L�X�g�̉摜��ǂݍ���
	CResourceManager::Load<CTexture>(INTERACT_TEXT_PATH, INTERACT_TEXT_PATH);

	// �w�肳�ꂽ�A�C�e���̎�ނ���A�C�e���f�[�^���擾
	bool success = Item::GetItemData(mItemTyope, &mpItemData);
	// �A�C�e���f�[�^�����݂��Ȃ�������A���g���폜
	if (!success)
	{
		Kill();
		return;
	}

	// �A�C�e���f�[�^�̃��f���f�[�^���擾
	//mpModel = CResourceManager::Get<CModel>(mpItemData->modelPath);

	mpItemImage = new CImage3D
	{
		"Item\\2D\\Light_of_the_item.png",
		ETag::eItem,
		ETaskPriority::eItem,0,
		ETaskPauseType::eGame,
		false,false
	};
	mpItemImage->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	mpItemImage->SetBillboard(true);

	// �R���C�_�[���쐬
	CreateCollider();

	// ���ׂ�Ƃ��ɕ\������e�L�X�g��ݒ�
	mInteractStr = "�E��";
}

// �f�X�g���N�^
CItemObj::~CItemObj()
{
	// �ǂݍ��񂾃C���[�W���폜
	SAFE_DELETE(mpItemImage);
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

// ���ׂ���e�̃e�L�X�g�摜�̃p�X��Ԃ�
std::string CItemObj::GetInteractTextPath() const
{
	return INTERACT_TEXT_PATH;
}

// ���ׂ�UI��\��������W��Ԃ�
CVector CItemObj::GetInteractUIPos() const
{
	return Position() + CVector(0.0f, 10.0f, 0.0f);
}

// �X�V����
void CItemObj::Update()
{
	mpItemImage->SetSize(mpItemImage->GetSize() - CVector2(1.0f, 1.0f) * Times::DeltaTime());

	mpItemImage->Rotation(mpItemImage->Rotation());

	mpItemImage->Position(Position());
	mpItemImage->Update();
}

// �`�揈��
void CItemObj::Render()
{
	mpItemImage->Render();
}

