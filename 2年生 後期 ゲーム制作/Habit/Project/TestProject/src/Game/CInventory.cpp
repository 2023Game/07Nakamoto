#include "CInventory.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CPlayer2.h"

#define INVENTORY_ALPHA 0.75f	// �A���t�@�l

#define INVENTORY_WIDTH 918.5f	// �C���x���g����X���W
#define INVENTORY_HEIGHT 360.0f	// �C���x���g����Y���W

#define ITEM_WIDTH 750.0f		// �A�C�e�����i�[����X���W
#define ITEM_HEIGHT 290.0f		// �A�C�e�����i�[����Y���W
#define ITEM_INTERVAL 60		// �A�C�e���̊Ԋu

#define SLOT_FRAME 5		// �A�C�e���X���b�g�̘g�̕�
#define SLOT_COLUMN 6		// �A�C�e���X���b�g�̗�
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
	//, mpItemData(nullptr)
	, mSelectIndex(0)
	, mIsOpened(false)
{
	spInstance = this;

	// �e�A�C�e���X���b�g�̃A�C�e���A�C�R���\���p�̃C���[�W���쐬
	int slotCount = SLOT_COUNT;
	for (int i = 0; i < slotCount; i++)
	{
		SlotData& slot = mItemSlots[i];
		slot.icon = new CImage
		(
			"", ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
			false, false
		);
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
		"UI\\inventory2.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpInventoryFrame->SetCenter(mpInventoryFrame->GetSize() * 0.5f);
	mpInventoryFrame->SetPos(CVector2(INVENTORY_WIDTH, INVENTORY_HEIGHT));

	// ����{�^��
	mpBackMenu = new CImage
	(
		"UI\\menu_item.png" ,
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackMenu->SetCenter(mpBackMenu->GetSize() * 0.5f);
	mpBackMenu->SetPos(CVector2(WINDOW_WIDTH * 0.7f, WINDOW_HEIGHT * 0.8f));
	mpBackMenu->SetColor(1.0f, 1.0f, 1.0f, INVENTORY_ALPHA);

	// �I������Ă���{�^������������g
	mpSelectFrame = new CImage
	(
		"UI/menu_item_select.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpSelectFrame->SetCenter(mpSelectFrame->GetSize() * 0.5f);
	mpSelectFrame->SetColor(1.0f, 0.5f, 0.0f, INVENTORY_ALPHA);

	

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
	SAFE_DELETE(mpBackMenu);
	SAFE_DELETE(mpSelectFrame);

	for (SlotData& slot : mItemSlots)
	{
		SAFE_DELETE(slot.icon);
	}
}

// �J��
void CInventory::Open()
{
	if (mIsOpened) return;

	// �}�E�X�J�[�\����\��
	CInput::ShowCursor(true);

	// �`���R �C���x���g���ɓ��鏈��
	//int ItemCount = 8;
	//for (int i = 0; i < ItemCount; i++)
	//{
	//	CImage* choco = new CImage
	//	(
	//		"Item\\choco\\choco.png",
	//		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
	//		false, false
	//	);

	//	choco->SetCenter(choco->GetSize() * 0.5f);

	//	int w = i % SLOT_COLUMN;
	//	int h = i / SLOT_COLUMN;
	//	float x = ITEM_WIDTH + SLOT_FRAME * (w + 1) + ITEM_INTERVAL * w;
	//	float y = ITEM_HEIGHT + SLOT_FRAME * (h + 1) + ITEM_INTERVAL * h;

	//	choco->SetPos(x, y);
	//	mItemSlots.push_back(choco);
	//}
	int i = 0;
	// �C���x���g���̒��g��\��
	for (SlotData& slot : mItemSlots)
	{
		if (slot.data != nullptr)
		{
			slot.icon->Load(slot.data->iconPath.c_str());
		}

		slot.icon->SetCenter(slot.icon->GetSize() * 0.5f);

		int w = i % SLOT_COLUMN;
		int h = i / SLOT_COLUMN;
		float x = ITEM_WIDTH + SLOT_FRAME * (w + 1) + ITEM_INTERVAL * w;
		float y = ITEM_HEIGHT + SLOT_FRAME * (h + 1) + ITEM_INTERVAL * h;

		slot.icon->SetPos(x, y);
			
		i++;
	}


	SetEnable(true);
	SetShow(true);
	mSelectIndex = 0;
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

// ���߂�
void CInventory::Decide(int select)
{
	switch (select)
	{
	case 0:
	case 1:
		break;
	case 2:
		Close();
		break;
	}
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

// �X�V
void CInventory::Update()
{
	mpBackground->Update();
	mpInventoryFrame->Update();
	mpBackMenu->Update();
	mpSelectFrame->Update();

	// �A�C�e���X���b�g�̃C���[�W�̍X�V
	for (SlotData& slot : mItemSlots)
	{
		slot.icon->Update();
	}
}

// �`��
void CInventory::Render()
{
	mpBackground->Render();
	mpInventoryFrame->Render();
	mpBackMenu->Render();

	mpSelectFrame->SetPos(mpBackMenu->GetPos());
	mpSelectFrame->Render();

	// �A�C�e���X���b�g��`��
	for (SlotData& slot : mItemSlots)
	{
		// ��̃X���b�g�͕`�悵�Ȃ�
		if (slot.data == nullptr) return;

		slot.icon->Render();
	}
}
