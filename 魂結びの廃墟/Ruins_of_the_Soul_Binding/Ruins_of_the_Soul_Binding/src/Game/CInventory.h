#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>
#include "ItemData.h"

class CItemSlotUI;
class CExpandButton;
class CItemMenu;
class SlotData;

// �C���x���g���̃N���X
class CInventory : public CTask
{
public:
	// �C���X�^���X���擾
	static CInventory* Instance();

	// �R���X�g���N�^
	CInventory();
	// �f�X�g���N�^
	~CInventory();

	// �J��
	void Open();
	// ����
	void Close();
	// �C���x���g�����J���Ă��邩�ǂ���
	bool IsOpened() const;

	// �A�C�e����ǉ�����
	void AddItem(ItemType type, int count);
	// �A�C�e���X���b�g�̃f�[�^���擾
	//const std::vector<SlotData>& GetItemSlotData() const;

	// �J�[�\�����X���b�g�ɏd�Ȃ���
	void EnterItemSlot(int index);
	// �J�[�\�����A�C�e���X���b�g���痣�ꂽ
	void ExitItemSlot(int index);
	// �A�C�e���X���b�g��͂�
	void GrabItemSlot(int index);
	// �͂�ł���A�C�e���X���b�g�𗣂���
	void ReleaseItemSlot(int index);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	static CInventory* spInstance;

	CImage* mpBackground;		// �C���x���g���̔w�i
	CImage* mpInventoryFrame;	// �C���x���g���̘g
	CImage* mpSlotHighlight;	// �J�[�\�����d�Ȃ��Ă���A�C�e���X���b�g�������\������
	CItemMenu* mpItemMenu;		// �A�C�e���I���̃��j���[��

	CImage* mpMenu;
	// �A�C�e���X���b�g�̃f�[�^
	struct SlotData
	{
		// ���̃X���b�g�ɓ����Ă���A�C�e���̃f�[�^
		const ItemData* data;
		int count;		// �����Ă���A�C�e���̌�
		CItemSlotUI* slotUI;	// �A�C�e���X���b�g��UI
		SlotData()
			: data(nullptr)
			, count(0)
			, slotUI(nullptr)
		{}
	};
	// �A�C�e���X���b�g�̃��X�g
	std::vector<SlotData> mItemSlots;

	bool mIsOpened;

	// �{�^���̃��X�g
	std::vector<CExpandButton*> mSlotButtons;

	int mEnterSlotIndex;		// �J�[�\�����d�Ȃ��Ă���A�C�e���̔ԍ�
	int mGrabSlotIndex;			// ���ݒ͂�ł���A�C�e���̔ԍ�
};