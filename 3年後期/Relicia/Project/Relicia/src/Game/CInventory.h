#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>
#include "ItemData.h"

class CItemSlotUI;
class CItemMenu;

#define ICON_SIZE 56.0f		// アイコンの大きさ

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

	// アイテムを追加する
	void AddItem(ItemType type, int count);

	// 指定された番号のアイテムスロットを返す
	const ItemData* GetItemSlotData(int slotIndex) const;

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

	CImage* mpMenuBg;			// メニュー背景
	CImage* mpInventoryFrame;	// インベントリの枠
	CImage* mpSlotHighlight;	// カーソルが重なっているアイテムスロットを強調表示する
	//CItemMenu* mpItemMenu;	// アイテム選択のメニュー覧

	// アイテムスロットのデータ
	struct SlotData
	{
		// そのスロットに入っているアイテムのデータ
		const ItemData* data;
		int count;		// 入っているアイテムの個数
		CItemSlotUI* slotUI;	// アイテムスロットのUI
		SlotData()
			: data(nullptr)
			, slotUI(nullptr)
		{}
	};
	// アイテムスロットのリスト
	std::vector<SlotData> mItemSlots;

	int mSlotRow;	// アイテムスロットの縦の数
	int mSlotCol;	// アイテムスロットの横の数

	// 開いているか
	bool mIsOpened;

	int mEnterSlotIndex;		// カーソルが重なっているアイテムの番号
	int mGrabSlotIndex;			// 現在掴んでいるアイテムの番号
};