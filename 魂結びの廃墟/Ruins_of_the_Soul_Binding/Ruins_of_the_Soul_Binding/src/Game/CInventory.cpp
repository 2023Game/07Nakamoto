#include "CInventory.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CPlayer2.h"
#include "CItemSlotUI.h"
#include "CExpandButton.h"
#include "CItemMenu.h"

#define INVENTORY_ALPHA 0.75f	// �A���t�@�l

#define INVENTORY_WIDTH 890.0f	// �C���x���g����X���W
#define INVENTORY_HEIGHT 360.0f	// �C���x���g����Y���W

#define ITEM_WIDTH 750.0f		// �A�C�e�����i�[����X���W
#define ITEM_HEIGHT 150.0f		// �A�C�e�����i�[����Y���W
#define ITEMSLOT_SIZE 139.0f		// �A�C�e���̃T�C�Y

#define SLOT_FRAME 3		// �A�C�e���X���b�g�̘g�̕�
#define SLOT_COLUMN 3		// �A�C�e���X���b�g�̗�
#define SLOT_ROW 3			// �A�C�e���X���b�g�̍s��

// �A�C�e���X���b�g��
#define SLOT_COUNT (SLOT_COLUMN * SLOT_ROW)

CInventory* CInventory::spInstance = nullptr;

// �C���X�^���X���擾
CInventory* CInventory::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CInventory::CInventory()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mItemSlots(SLOT_COUNT)
	, mSlotButtons(SLOT_COLUMN)
	, mIsOpened(false)
	, mEnterSlotIndex(-1)
	, mGrabSlotIndex(-1)
	, mpSlotHighlight(nullptr)
	, mpItemMenu(nullptr)
	, mpMenu(nullptr)
{
	spInstance = this;

	// �e�A�C�e���X���b�g�̃A�C�e���A�C�R���\���p�̃C���[�W���쐬
	int slotCount = SLOT_COUNT;
	for (int i = 0; i < slotCount; i++)
	{
		SlotData& slot = mItemSlots[i];
		slot.slotUI = new CItemSlotUI(i);
	}

	// �C���x���g���̔w�i
	mpBackground = new CImage
	(
		"UI\\menu_back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(1.0f, 1.0f, 1.0f, INVENTORY_ALPHA);

	// �C���x���g���̘g
	mpInventoryFrame = new CImage
	(
		"UI\\StatusScreen\\Item_slot.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpInventoryFrame->SetCenter(mpInventoryFrame->GetSize() * 0.5f);
	mpInventoryFrame->SetPos(CVector2(INVENTORY_WIDTH, INVENTORY_HEIGHT));

	// �J�[�\�����d�Ȃ��Ă���A�C�e���X���b�g�������\������
	mpSlotHighlight = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpSlotHighlight->SetSize(ITEMSLOT_SIZE, ITEMSLOT_SIZE);
	mpSlotHighlight->SetCenter(mpSlotHighlight->GetSize() * 0.5f);
	mpSlotHighlight->SetColor(CColor::yellow);
	mpSlotHighlight->SetAlpha(0.5f);
	mpSlotHighlight->SetEnable(false);

	// �I�������A�C�e���̃��j���[��
	mpItemMenu = new CItemMenu();

	SetEnable(false);
	SetShow(false);
}

// �f�X�g���N�^
CInventory::~CInventory()
{
	if (spInstance == this)
	{
		spInstance = nullptr;
	}

	SAFE_DELETE(mpBackground);
	SAFE_DELETE(mpInventoryFrame);
	SAFE_DELETE(mpSlotHighlight);
	SAFE_DELETE(mpItemMenu);

	for (SlotData& slot : mItemSlots)
	{
		SAFE_DELETE(slot.slotUI);
	}
}

// �J��
void CInventory::Open()
{
	if (mIsOpened) return;

	// �}�E�X�J�[�\����\��
	CInput::ShowCursor(true);

	int size = mItemSlots.size();
	// �A�C�e���X���b�g�̈ʒu�̐ݒ�
	for (int i = 0; i < size; i++)
	{
		SlotData& slot = mItemSlots[i];

		if (slot.data != nullptr)
		{
			slot.slotUI->SetItemSloto(slot.data, slot.count);
		}
		else
		{
			slot.slotUI->SetItemSloto(nullptr, -1);
		}

		int w = i % SLOT_COLUMN;
		int h = i / SLOT_COLUMN;
		float x = ITEM_WIDTH + SLOT_FRAME * (w + 1) + ITEMSLOT_SIZE * w;
		float y = ITEM_HEIGHT + SLOT_FRAME * (h + 1) + ITEMSLOT_SIZE * h;

		slot.slotUI->SetPos(x, y);
	}

	SetEnable(true);
	SetShow(true);
	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);

	mIsOpened = true;
}

// ����
void CInventory::Close()
{
	if (!mIsOpened) return;

	// �}�E�X�J�[�\�����\��
	CInput::ShowCursor(false);

	SetEnable(false);
	SetShow(false);
	CBGMManager::Instance()->Play(EBGMType::eGame, false);
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);

	mIsOpened = false;
}

// �C���x���g�����J���Ă��邩�ǂ���
bool CInventory::IsOpened() const
{
	return mIsOpened;
}

// �A�C�e����ǉ�����
void CInventory::AddItem(ItemType type, int count)
{
	// �A�C�e���f�[�^���擾�ł��Ȃ�������A�ǉ��ł��Ȃ�
	const ItemData* itemData = nullptr;
	bool success = Item::GetItemData(type, &itemData);
	if (!success) return;

	// �ǉ�����A�C�e���̎c���
	int remain = count;

	// �����ɃA�C�e���������Ă���X���b�g�ɃA�C�e����ǉ�����
	// �A�C�e���X���b�g�̒��g���m�F���āA
	// ������ނ̃A�C�e���������Ă���A�C�e���X���b�g��T��
	for (SlotData& slot : mItemSlots)
	{
		// ��̃A�C�e���X���b�g�ł���΁A���̃A�C�e���X���b�g�𒲂ׂ�
		if (slot.data == nullptr) continue;
		// �A�C�e���X���b�g�ɓ����Ă���A�C�e���̎�ނ�
		// �ǉ�����A�C�e���̎�ނ���v���Ȃ���΁A���̃A�C�e���X���b�g�𒲂ׂ�
		if (slot.data->type != type) continue;

		// �A�C�e���̎�ނ���v����A�C�e���X���b�g����������

		//�@�ǉ���̃A�C�e�������Z�o
		int sum = slot.count + remain;

		// �ǉ���̃A�C�e����������𒴂���ꍇ�́A
		if (sum > slot.data->slotLimit)
		{
			// ��ꂽ�A�C�e�������v�Z
			remain = sum - slot.data->slotLimit;
			// �ݒ肷��A�C�e������������ɐݒ�
			sum = slot.data->slotLimit;
		}
		// ����𒴂��Ȃ������ꍇ�́A�c�����0�ɂ���
		else
		{
			remain = 0;
		}
		// ���������A�C�e���X���b�g�̃A�C�e������ݒ�
		slot.count = sum;

		// �c�����0�ł���΁A�������I����
		if (remain == 0)
		{
			break;
		}
	}

	// ����̃X���b�g�ɃA�C�e����ǉ�����
	// �܂��ǉ�����A�C�e�����c���Ă����ꍇ
	if (remain > 0)
	{
		// �A�C�e���X���b�g�̃��X�g�̐擪�����̃X���b�g��T���āA
		// ����������̃A�C�e���X���b�g�ɃA�C�e��������
		for (SlotData& slot : mItemSlots)
		{
			// ���ɃA�C�e���������Ă���X���b�g�̓X���[
			if (slot.data != nullptr) continue;

			// ����������̃��X�g�ɁB�ǉ�����A�C�e���̃f�[�^��ݒ�
			slot.data = itemData;

			// �ǉ�����A�C�e���̌�������𒴂��Ă�����A���̃X���b�g�Ɏc��
			int itemCount = remain;
			if (itemCount > slot.data->slotLimit)
			{
				remain = itemCount - slot.data->slotLimit;
				itemCount = slot.data->slotLimit;
			}
			else
			{
				remain = 0;
			}
			slot.count = itemCount;

			// �ǉ�����A�C�e���̌����c���ĂȂ���΁A�ǉ������I��
			if (remain == 0)
			{
				break;
			}
		}
	}

	// �܂��ǉ�����A�C�e�����c���Ă����ꍇ
	// �A�C�e���X���b�g�������ς��̏ꍇ
	if (remain > 0)
	{
		// TODO�F�C���x���g���ɒǉ��ł��Ȃ������A�C�e���̓h���b�v����

		// ��U�폜���Ă���
		remain = 0;
	}
}

// �J�[�\�����X���b�g�ɏd�Ȃ���
void CInventory::EnterItemSlot(int index)
{
	// �J�[�\�����d�Ȃ��Ă���A�C�e���X���b�g�̔ԍ����X�V
	mEnterSlotIndex = index;
}

// �J�[�\�����A�C�e���X���b�g���痣�ꂽ
void CInventory::ExitItemSlot(int index)
{
	// �J�[�\�����d�Ȃ��Ă���A�C�e���X���b�g�̔ԍ��ƈ�v����΁A
	// �d�Ȃ��Ă��Ȃ���Ԃ֖߂�
	if (mEnterSlotIndex == index)
	{
		mEnterSlotIndex = -1;
	}
}

// �A�C�e���X���b�g��͂�
void CInventory::GrabItemSlot(int index)
{
	mGrabSlotIndex = index;
}

// �͂�ł���A�C�e���X���b�g�𗣂���
void CInventory::ReleaseItemSlot(int index)
{
	// �����ꂽ�A�C�e���X���b�g�ƒ͂�ł���A�C�e���X���b�g���Ⴄ�ꍇ�́A�������Ȃ�
	if (mGrabSlotIndex != index) return;
	// ���̃A�C�e�����͂�łȂ���΁A�������Ȃ�
	if (mGrabSlotIndex == -1) return;
	// �J�[�\�����ǂ̃A�C�e���X���b�g�ɂ��d�Ȃ��Ă��Ȃ��ꍇ�́A�������Ȃ�
	if (mEnterSlotIndex == -1) return;
	// �͂�ł����A�C�e���X���b�g��
	// �J�[�\�����d�Ȃ��Ă���A�C�e���X���b�g�������ł���΁A�������Ȃ�
	if (mGrabSlotIndex == mEnterSlotIndex) return;

	SlotData& grabData = mItemSlots[mGrabSlotIndex];
	SlotData& enterData = mItemSlots[mEnterSlotIndex];

	// �͂�ł���A�C�e���̃X���b�g�̏���ێ����Ă���
	const ItemData* tmpData = grabData.data;
	int tmpCount = grabData.count;

	// �͂�ł����A�C�e���X���b�g�ɁA
	// �J�[�\�����d�Ȃ��Ă����A�C�e���X���b�g�̃f�[�^������
	grabData.data = enterData.data;
	grabData.count = enterData.count;
	grabData.slotUI->SetItemSloto(grabData.data, grabData.count);

	// �J�[�\�����d�Ȃ��Ă����A�C�e���X���b�g�ɂ́A
	// �͂�ł����A�C�e���X���b�g�̃f�[�^������
	enterData.data = tmpData;
	enterData.count = tmpCount;
	enterData.slotUI->SetItemSloto(enterData.data, enterData.count);

	// �͂�ł�����Ԃ�����
	mGrabSlotIndex = -1;
}

// �X�V
void CInventory::Update()
{
	// �A�C�e�����j���[���\������Ă��Ȃ��ꍇ
	if (!mpItemMenu->IsShow())
	{
		mpBackground->Update();
		mpInventoryFrame->Update();

		// �A�C�e���X���b�g�̃C���[�W�̍X�V
		for (SlotData& slot : mItemSlots)
		{
			slot.slotUI->Update();
		}

		if (mEnterSlotIndex != -1)
		{
			SlotData& itemData = mItemSlots[mEnterSlotIndex];

			// �A�C�e���A�C�R���̏�ŉE�N���b�N������
			if (CInput::Key(VK_RBUTTON) && !mpItemMenu->IsShow())
			{
				// �A�C�e���A�C�R��������ꍇ
				if (itemData.data != nullptr)
				{
					mpItemMenu->SetPos(itemData.slotUI->GetPos());
					// �A�C�e�����j���[��\������
					mpItemMenu->Open();
					mpItemMenu->Update();
				}
			}
			else
			{
				mpSlotHighlight->SetPos(itemData.slotUI->GetPos());
				mpSlotHighlight->SetEnable(true);
				mpSlotHighlight->Update();
			}
		}
		else if (mEnterSlotIndex == -1)
		{
			mpSlotHighlight->SetEnable(false);
		}
	}
	// �A�C�e�����j���[���\������Ă��Ȃ����
	else
	{
		mpItemMenu->Update();

		// �u�g���v���������ꍇ
		if (mpItemMenu->IsUse())
		{
			CPlayer2* player = CPlayer2::Instance();
			SlotData& slot = mItemSlots[mEnterSlotIndex];

			// �A�C�e�����g�p�ł��邩�ǂ���
			if (player->CanUseItem(slot.data))
			{
				// �A�C�e�����g�p����O�ɁA�A�C�e�����j���[�ƃC���x���g�������
				mpItemMenu->Close();
				Close();

				// �A�C�e�����g�p
				player->UseItem(slot.data);
				// �g�p�����A�C�e���̌������炷
				slot.count--;
				// �A�C�e���������Ȃ�΁A�A�C�e���X���b�g����ɂ���
				if (slot.count == 0)
				{
					slot.data = nullptr;
				}
				// �A�C�e���X���b�g�̏���UI�ɔ��f
				slot.slotUI->SetItemSloto(slot.data, slot.count);
			}
			// �A�C�e�����g�p�ł��Ȃ������ꍇ
			else
			{
				// TODO:�A�C�e�����g�p�ł��Ȃ��������R����ʂɕ\��
				mpItemMenu->Close();
			}
		}
		// �u����v���������ꍇ
		else if (mpItemMenu->IsClose())
		{
			mpItemMenu->Close();
		}
	}
}

// �`��
void CInventory::Render()
{
	mpBackground->Render();
	mpInventoryFrame->Render();

	if (mpSlotHighlight->IsEnable())
	{
		mpSlotHighlight->Render();
	}

	// �A�C�e���X���b�g��`��
	int count = mItemSlots.size();
	for (int i = 0; i < count; i++)
	{
		SlotData& slot = mItemSlots[i];
		// ��̃X���b�g�͕`�悵�Ȃ�
		if (slot.data == nullptr) continue;
		// ���ݒ͂�ł���A�C�e���X���b�g�͂��̃^�C�~���O�ł͕`�悵�Ȃ�
		if (i == mGrabSlotIndex) continue;

		slot.slotUI->Render();
	}

	// �A�C�e���X���b�g��͂�ł���ꍇ
	if (mGrabSlotIndex >= 0)
	{
		// ��X���b�g�łȂ���΁A���̃^�C�~���O�ŕ`��
		SlotData& slot = mItemSlots[mGrabSlotIndex];
		if (slot.data != nullptr)
		{
			slot.slotUI->Render();
		}
	}

	// �A�C�e�����j���[���L���Ȃ�
	if (mpItemMenu->IsEnable())
	{
		// �`�悷��
		mpItemMenu->Render();
	}

}
