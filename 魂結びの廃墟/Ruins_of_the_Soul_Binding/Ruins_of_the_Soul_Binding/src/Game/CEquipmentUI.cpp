#include "CEquipmentUI.h"
#include "CImage.h"
#include "CInventory.h"

// �R���X�g���N�^
CEquipmentUI::CEquipmentUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpEquipmentSlot1(nullptr)
	, mpEquipItemImage(nullptr)
{
	// �����A�C�e���̃X���b�g�̐���
	mpEquipmentSlot1 = new CImage
	(
		"UI\\EquipmentSlot2.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mpEquipmentSlot1->SetCenter(mpEquipmentSlot1->GetSize() * 0.5f);

	// �������Ă���A�C�e���̃C���[�W�ǂݍ���
	mpEquipItemImage = new CImage
	(
		"",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mpEquipItemImage->SetCenter(mpEquipmentSlot1->GetSize() * 0.5f);
}

// �f�X�g���N�^
CEquipmentUI::~CEquipmentUI()
{
	SAFE_DELETE(mpEquipmentSlot1);
}

// �w�肵���A�C�e���X���b�g�̃A�C�e���𑕔�
void CEquipmentUI::EquipItem(const ItemData* item)
{
	// �A�C�e���̃f�[�^���ݒ肳�ꂽ��
	if (item != nullptr)
	{
		// �ݒ肳�ꂽ�A�C�e���̃A�C�R����ǂݍ���ŕ\��
		mpEquipItemImage->Load(item->iconPath.c_str());
		mpEquipItemImage->SetShow(true);
	}
	else
	{
		// �A�C�e���̃f�[�^���󂾂�����
		mpEquipItemImage->Load("");
		mpEquipItemImage->SetShow(false);
	}
}

// �X�V
void CEquipmentUI::Update()
{
}

// �`��
void CEquipmentUI::Render()
{
	// �����A�C�e���̘g��`��
	mpEquipmentSlot1->SetAlpha(GetAlpha());
	mpEquipmentSlot1->SetPos(mPosition);
	mpEquipmentSlot1->Render();

	// �������Ă�����A�����A�C�R���̃A�C�e����`��
	if (mpEquipItemImage->IsShow())
	{
		mpEquipItemImage->SetAlpha(GetAlpha());
		mpEquipItemImage->SetPos(mPosition);
		mpEquipItemImage->Render();
	}
}
