#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>
#include "ItemData.h"

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
	CImage* mpTexture;

	// アイテムスロットのデータ
	struct SlotData
	{
		// そのスロットに入っているアイテムのデータ
		const ItemData* data;
		int count;		// 入っているアイテムの個数
		CImage* icon;	// アイコンのイメージ
		SlotData()
			: data(nullptr)
			, count(0)
			, icon(nullptr)
		{}
	};
	// アイテムスロットのリスト
	std::vector<SlotData> mItemSlots;

	//ItemType mItemTyope;	// アイテムの種類
	//const ItemData* mpItemData;	// アイテムデータのポインタ
	// CTexture* mpTexture;	//表示するイメージのテクスチャ
	int mSelectIndex;
	bool mIsOpened;
};