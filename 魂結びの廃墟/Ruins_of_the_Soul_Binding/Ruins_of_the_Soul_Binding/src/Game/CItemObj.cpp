#include "CItemObj.h"
#include "CColliderSphere.h"
#include "CInventory.h"
#include "CImage3D.h"
#include "Maths.h"

#define INTERACT_TEXT_PATH "UI\\Interact\\pickup.png"
#define WORLD_UNIT_PER_PIXEL 80.0f

// 1�b�Ԃŉ�]����p�x
#define ROTATE_SPEED 90.0f

// �g�k�A�j���[�V�����̎���
#define SCALING_TIME 2.0f
// �g�k�A�j���[�V�������̃X�P�[���ŏ��l
#define SCALING_MIN 0.75f
// �g�k�A�j���[�V�������̃X�P�[���ő�l
#define SCALING_MAX 1.25f

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

	// �����_���ŏ��ׂ̉�]�p�x�����߂�
	float angleZ = Math::Rand(0.0f, 360.0f);
	mpItemImage->Rotation(0.0f, 0.0f, angleZ);
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
	// ���Ԍo�߂ɍ��킹�Ċg�k�A�j���[�V�������Đ�
	float alpha = fmodf(Times::Time(), SCALING_TIME) / SCALING_TIME;
	float t = (sinf(M_PI * 2.0f * alpha) + 1.0f) * 0.5f;
	float scale = Math::Lerp(SCALING_MIN, SCALING_MAX, t);
	mpItemImage->Scale(scale,scale,scale);

	// Z����]
	mpItemImage->Rotate(0.0f, 0.0f, ROTATE_SPEED * Times::DeltaTime());

	mpItemImage->Position(Position());
	mpItemImage->Update();
}

// �`�揈��
void CItemObj::Render()
{
	mpItemImage->Render();
}

