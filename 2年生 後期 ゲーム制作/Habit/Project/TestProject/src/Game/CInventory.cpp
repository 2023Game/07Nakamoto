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

	//int size = mItemList.size();
	//for (int i = 0; i < size; i++)
	//{
	//	CImage* item = mItemList[i];
	//	mItemList[i] = nullptr;
	//	SAFE_DELETE(item);
	//}
	//mItemList.clear();
}

// �J��
void CInventory::Open()
{
	if (mIsOpened) return;

	// �}�E�X�J�[�\����\��
	CInput::ShowCursor(true);

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


	//for (CImage* item : mItemList)
	//{
	//	item->Update();
	//}
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
		if (slot.data = nullptr) return;

		slot.icon->Render();
	}
	//for (int i = 0; i < mItemList.size(); i++)
	//{
	//	CImage* item = mItemList[i];
	//	item->Render();
	//}
}
