#include "CItemSlotUI.h"
#include "CImage.h"
#include "CText.h"
#include "CInventory.h"

#define COUNT_TEXT_POS CVector2(15.0f, 0.0f)
#define SLOT_SIZE 60.0f

// �R���X�g���N�^
CItemSlotUI::CItemSlotUI(int slotIdx)
	: CUIBase(ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false)
	, mSlotIndex(slotIdx)
	, mpItemData(nullptr)
	, mpIcon(nullptr)
	, mpCountText(nullptr)
	, mpSlotHighlight(nullptr)
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

	mpSlotHighlight = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);

	mpSlotHighlight->SetColor(CColor::yellow);
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
	mpIcon->SetPos(mPosition);
}

// �J�[�\�����d�Ȃ�����
void CItemSlotUI::OnPointerEnter(const CVector2& pos)
{
	// �C���x���g���ɃJ�[�\�����d�Ȃ������Ƃ�`����
	CInventory::Instance()->EnterItemSlot(mSlotIndex);
}

// �J�[�\�������ꂽ��
void CItemSlotUI::OnPointerExit(const CVector2& pos)
{
	// �C���x���g���ɃJ�[�\�������ꂽ���Ƃ�`����
	CInventory::Instance()->ExitItemSlot(mSlotIndex);
}

// �N���b�N���ꂽ��
void CItemSlotUI::OnPointerDown(const CVector2& pos)
{
	// ��X���b�g�̏ꍇ�͒͂߂Ȃ�
	if (mpItemData == nullptr) return;

	// �C���x���g���ɃA�C�e���X���b�g���͂܂ꂽ���Ƃ�`����
	CInventory::Instance()->GrabItemSlot(mSlotIndex);
}

// �N���b�N�𗣂����Ƃ�
void CItemSlotUI::OnPointerUp(const CVector2& pos)
{
	// ��X���b�g�̏ꍇ�͗����Ȃ�
	if (mpItemData == nullptr) return;

	// �C���x���g���ɃA�C�e���X���b�g�������ꂽ���Ƃ�`����
	CInventory::Instance()->ReleaseItemSlot(mSlotIndex);
}

// �h���b�N&�h���b�v�̈ړ�����
void CItemSlotUI::OnMove(const CVector2& move)
{
	if (mpItemData == nullptr) return;
#if _DEBUG
	CDebugPrint::Print("Move:%s: %.2f, %.2f\n", mpItemData->name.c_str(), move.X(), move.Y());
#endif

	// �A�C�e���A�C�R���̈ړ�
	mpIcon->SetPos(mpIcon->GetPos() + move);
	mpCountText->SetPos(mpIcon->GetPos() + COUNT_TEXT_POS);
	//printf("%.2f, %.2f\n", mpIcon->GetPos().X(), mpIcon->GetPos().Y());
}

// �X�V
void CItemSlotUI::Update()
{
	CUIBase::Update();
	
	mpIcon->Update();
	mpCountText->Update();

	if (mpItemData != nullptr)
	{
		// �A�C�R���̏�Œ��������Ă���ꍇ
		if (mIsTouch)
		{
#if _DEBUG
			CDebugPrint::Print("Touch:%s\n", mpItemData->name.c_str());
			//mpItemAddress = mpItemData;
			CDebugPrint::Print("Touch:%p\n", mpItemData);
#endif

			if (mpIcon->GetSize() == CVector2(SLOT_SIZE, SLOT_SIZE))
			{
				// �A�C�e�����h���b�O���̃A�C�e���������傫������
				mpIcon->SetSize(mpIcon->GetSize() * 1.1f);
			}
		}
		// �N���b�N�𗣂����ꍇ
		else
		{
			// �A�C�e�������̈ʒu�ɖ߂�
			mpIcon->SetSize(SLOT_SIZE, SLOT_SIZE);
			mpIcon->SetPos(mPosition);
			mpCountText->SetPos(mPosition + COUNT_TEXT_POS);

		}

		// �A�C�e���A�C�R���̏�ɃJ�[�\��������ꍇ
		if (mIsEnter)
		{
#if _DEBUG
			CDebugPrint::Print("Enter:%s\n", mpItemData->name.c_str());
			CDebugPrint::Print("Enter:%p\n", mpItemData);
#endif
		}
		else
		{

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
