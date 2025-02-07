#pragma once
#include "CUIBase.h"
#include "CImage.h"
#include "ItemData.h"

class CItemSlotUI : public CUIBase
{
public:
	// コンストラクタ
	CItemSlotUI(int slotIdx);
	// デストラクタ
	~CItemSlotUI();

	/// <summary>
	/// アイテムスロットの設定
	/// </summary>
	/// <param name="data">スロットに入っているアイテムのデータ</param>
	/// <param name="count">スロットに入っているアイテムの個数</param>
	void SetItemSloto(const ItemData* data, int count);

	void OnPointerEnter(const CVector2& pos) override;
	void OnPointerExit(const CVector2& pos) override;
	void OnPointerDown(const CVector2& pos) override;
	void OnPointerUp(const CVector2& pos) override;
	void OnMove(const CVector2& move) override;

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
	int mSlotIndex;		// 何番目のアイテムスロットか
	const ItemData* mpItemData;
	CImage* mpIcon;		// アイテムアイコンのイメージ
	CText* mpCountText;	// アイテムの個数表示用のテキスト
	CVector2 mCursor_pos;	// カーソルの位置

	CImage* mpSlotHighlight;	// カーソルが重なっているアイテムスロットを強調表示する
};