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
	// コンストラクタ
	CEquipmentUI();
	// デストラクタ
	~CEquipmentUI();



	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CImage* mpEquipmentSlot1;	// 装備スロット枠のイメージ
	CImage* mpEquipmentSlot2;	// 装備スロット枠のイメージ
	CImage* mpEquipmentSlot3;	// 装備スロット枠のイメージ

	//// アイテムスロットのデータ
	//struct SlotData
	//{
	//	// そのスロットに入っているアイテムのデータ
	//	const ItemData* data;
	//	int count;		// 入っているアイテムの個数
	//	CItemSlotUI* slotUI;	// アイテムスロットのUI
	//	SlotData()
	//		: data(nullptr)
	//		, count(0)
	//		, slotUI(nullptr)
	//	{}
	//};
	//// アイテムスロットのリスト
	//std::vector<SlotData> mItemSlots;
};