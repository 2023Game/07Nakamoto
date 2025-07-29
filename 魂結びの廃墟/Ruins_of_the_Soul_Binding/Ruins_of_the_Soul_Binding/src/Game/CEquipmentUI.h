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
	// コンストラクタ
	CEquipmentUI();
	// デストラクタ
	~CEquipmentUI();

	// 指定したアイテムを装備
	void EquipItem(const ItemData* item);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CImage* mpEquipmentSlot1;	// 装備スロット枠のイメージ
	CImage* mpEquipItemImage;	// 装備しているアイテムのイメージ
};