#pragma once
#include "CUIBase.h"
#include "CImage.h"
#include "ItemData.h"

class CItemSlotUI : public CUIBase
{
public:
	// コンストラクタ
	CItemSlotUI();
	// デストラクタ
	~CItemSlotUI();

	/// <summary>
	/// アイテムスロットの設定
	/// </summary>
	/// <param name="data">スロットに入っているアイテムのデータ</param>
	/// <param name="count">スロットに入っているアイテムの個数</param>
	void SetItemSloto(const ItemData* data, int count);

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
	CImage* mpIcon;		// アイテムアイコンのイメージ
	CText* mpCountText;	// アイテムの個数表示用のテキスト
};