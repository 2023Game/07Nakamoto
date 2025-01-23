#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>
#include "ItemData.h"

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

	// ���߂�
	void Decide(int select);
	// �A�C�e����ǉ�����
	void AddItem(ItemType type, int count);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	static CInventory* spInstance;

	CImage* mpBackground;
	CImage* mpInventoryFrame;
	CImage* mpBackMenu;
	CImage* mpSelectFrame;

	// �A�C�e���X���b�g�̃f�[�^
	struct SlotData
	{
		// ���̃X���b�g�ɓ����Ă���A�C�e���̃f�[�^
		const ItemData* data;
		int count;		// �����Ă���A�C�e���̌�
		CImage* icon;	// �A�C�R���̃C���[�W
		SlotData()
			: data(nullptr)
			,count(0)
			, icon(nullptr)
		{}
	};
	// �A�C�e���X���b�g�̃��X�g
	std::vector<SlotData> mItemSlots;

	int mSelectIndex;
	bool mIsOpened;
};