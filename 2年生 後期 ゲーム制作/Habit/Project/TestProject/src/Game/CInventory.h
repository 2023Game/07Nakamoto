#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>
#include "ItemData.h"

class CItemSlotUI;
class CExpandButton;

// インベントリのクラス
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

	// カーソルがスロットに重なった
	void EnterItemSlot(int index);
	// カーソルがアイテムスロットから離れた
	void ExitItemSlot(int index);
	// アイテムスロットを掴んだ
	void GrabItemSlot(int index);
	// 掴んでいるアイテムスロットを離した
	void ReleaseItemSlot(int index);

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
	CImage* mpSlotHighlight;	// カーソルが重なっているアイテムスロットを強調表示する
	CImage* mpItemMenu;			// アイテム選択のメニュー覧

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

	bool mIsOpened;

	// ボタンのリスト
	std::vector<CExpandButton*> mSlotButtons;

	int mEnterSlotIndex;		// カーソルが重なっているアイテムの番号
	int mGrabSlotIndex;			// 現在掴んでいるアイテムの番号
};