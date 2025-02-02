#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>
#include "ItemData.h"

class CItemSlotUI;
class CExpandButton;

class CInventory : public CTask
{
public:
	// インスタンスを取得
	static CInventory* Instance();

	// コンストラクタ
	CInventory();
	// デストラクタ
	~CInventory();

	// 開く
	void Open();
	// 閉じる
	void Close();
	// インベントリが開いているかどうか
	bool IsOpened() const;

	// 決める
	void Decide(int select);
	// アイテムを追加する
	void AddItem(ItemType type, int count);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	static CInventory* spInstance;

	CImage* mpBackground;
	CImage* mpInventoryFrame;
	CImage* mpBackMenu;
	CImage* mpSelectFrame;

	// アイテムスロットのデータ
	struct SlotData
	{
		// そのスロットに入っているアイテムのデータ
		const ItemData* data;
		int count;		// 入っているアイテムの個数
		CItemSlotUI* slotUI;	// アイテムスロットのUI
		SlotData()
			: data(nullptr)
			, count(0)
			, slotUI(nullptr)
		{}
	};
	// アイテムスロットのリスト
	std::vector<SlotData> mItemSlots;

	// アイテムスロットが開いているかどうか
	bool mIsOpened;

	// ボタンのリスト
	std::vector<CExpandButton*> mSlotButtons;
};