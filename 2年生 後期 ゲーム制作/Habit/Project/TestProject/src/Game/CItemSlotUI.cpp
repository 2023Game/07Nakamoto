#include "CItemSlotUI.h"
#include "CImage.h"
#include "CText.h"

#define COUNT_TEXT_POS CVector2(15.0f, 0.0f)
#define SLOT_SIZE 60.0f

// �R���X�g���N�^
CItemSlotUI::CItemSlotUI()
	: CUIBase(ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false)
	, mpItemData(nullptr)
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
	mpItemData = data;

	CVector2 size(SLOT_SIZE, SLOT_SIZE);

	if (data != nullptr)
	{
		mpIcon->Load(data->iconPath.c_str());
		mpIcon->SetCenter(mpIcon->GetSize() * 0.5f);

		// �����ʒu�ɕ���������ꍇ��
		if (data->slotLimit > 1)
		{
			// ���̃e�L�X�g��ݒ肷��
			mpCountText->SetText("%d", count);
		}
	}
	else
	{
		mpIcon->Load("");
		mpCountText->SetText("");
	}

	SetSize(size);
	SetCenter(size * 0.5);
	
}

void CItemSlotUI::OnPointerEnter(const CVector2& pos)
{
}

void CItemSlotUI::OnPointerExit(const CVector2& pos)
{
}

void CItemSlotUI::OnPointerDown(const CVector2& pos)
{
}

void CItemSlotUI::OnPointerUp(const CVector2& pos)
{
}

void CItemSlotUI::OnMove(const CVector2& move)
{
	if (mpItemData == nullptr) return;
	CDebugPrint::Print("Move:%s: %.2f, %.2f\n", mpItemData->name.c_str(), move.X(), move.Y());

	//mpIcon->SetPos(mpIcon->GetPos() + move);
}

// �X�V
void CItemSlotUI::Update()
{
	CUIBase::Update();

	mpIcon->SetPos(mPosition);
	mpCountText->SetPos(mPosition + COUNT_TEXT_POS);

	mpIcon->Update();
	mpCountText->Update();

	if (mpItemData != nullptr)
	{
		// �A�C�R���̏�ɃJ�[�\�������邩
		if (mIsEnter)
		{
			CDebugPrint::Print("Enter:%s\n", mpItemData->name.c_str());
		}

		// �A�C�R���̏�ŃN���b�N���Ă��邩
		if (mIsTouch)
		{
			CDebugPrint::Print("Touch:%s\n", mpItemData->name.c_str());
			if (mpIcon->GetSize() == CVector2(SLOT_SIZE, SLOT_SIZE))
			{
				mpIcon->SetSize(mpIcon->GetSize() * 1.1f);
			}
		}
		else
		{
			mpIcon->SetSize(SLOT_SIZE, SLOT_SIZE);
		}
	}
}

// �`��
void CItemSlotUI::Render()
{
	if (mpItemData == nullptr) return;
	mpIcon->Render();
	mpCountText->Render();
}
