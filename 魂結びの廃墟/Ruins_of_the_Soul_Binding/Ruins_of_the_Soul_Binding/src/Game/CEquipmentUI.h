#pragma once
#include "CTexture.h"
#include "CUIBase.h"
#include "ItemData.h"

class CItemSlotUI;
class CImage;
class SlotData;
struct ItemData;

class CEquipmentUI : public CUIBase
{
public:
	// �R���X�g���N�^
	CEquipmentUI();
	// �f�X�g���N�^
	~CEquipmentUI();

	// �w�肵���A�C�e���𑕔�
	void EquipItem(const ItemData* item);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CImage* mpEquipmentSlot1;	// �����X���b�g�g�̃C���[�W
	CImage* mpEquipItemImage;	// �������Ă���A�C�e���̃C���[�W
};