#pragma once
#include "CTexture.h"
#include "CUIBase.h"
#include "ItemData.h"

class CItemSlotUI;
class CImage;
class SlotData;

class CEquipmentUI : public CUIBase
{
public:
	// �R���X�g���N�^
	CEquipmentUI();
	// �f�X�g���N�^
	~CEquipmentUI();



	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CImage* mpEquipmentSlot;	// �����X���b�g�g�̃C���[�W

	//// �A�C�e���X���b�g�̃f�[�^
	//struct SlotData
	//{
	//	// ���̃X���b�g�ɓ����Ă���A�C�e���̃f�[�^
	//	const ItemData* data;
	//	int count;		// �����Ă���A�C�e���̌�
	//	CItemSlotUI* slotUI;	// �A�C�e���X���b�g��UI
	//	SlotData()
	//		: data(nullptr)
	//		, count(0)
	//		, slotUI(nullptr)
	//	{}
	//};
	//// �A�C�e���X���b�g�̃��X�g
	//std::vector<SlotData> mItemSlots;
};