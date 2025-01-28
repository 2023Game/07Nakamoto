#include "CItemSlotUI.h"
#include "CImage.h"
#include "CText.h"

#define COUNT_TEXT_POS CVector2(15.0f, 0.0f)

// �R���X�g���N�^
CItemSlotUI::CItemSlotUI()
	: CUIBase(ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false)
	, mpIcon(nullptr)
	, mpCountText(nullptr)
{
	mpIcon = new CImage
	(
		"", ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);

	mpCountText = new CText
	(
		nullptr, 24,
		CVector2::zero, CVector2(400.0f, 300.0f),
		CColor::black,
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpCountText->SetEnableOutline(true);
	mpCountText->SetOutlineColor(CColor::white);

}

// �f�X�g���N�^
CItemSlotUI::~CItemSlotUI()
{
	SAFE_DELETE(mpIcon);
	SAFE_DELETE(mpCountText);
}

// �A�C�e���X���b�g�̐ݒ�
void CItemSlotUI::SetItemSloto(const ItemData* data, int count)
{
	if (data == nullptr) return;
	mpIcon->Load(data->iconPath.c_str());
	mpIcon->SetCenter(mpIcon->GetSize() * 0.5f);

	// �����ʒu�ɕ���������ꍇ��
	if (data->slotLimit > 1)
	{
		// ���̃e�L�X�g��ݒ肷��
		mpCountText->SetText("%d", count);
	}
}

// �X�V
void CItemSlotUI::Update()
{
	CUIBase::Update();

	mpIcon->SetPos(mPosition);
	mpCountText->SetPos(mPosition + COUNT_TEXT_POS);

	mpIcon->Update();
	mpCountText->Update();
}

// �`��
void CItemSlotUI::Render()
{
	mpIcon->Render();
	mpCountText->Render();
}
