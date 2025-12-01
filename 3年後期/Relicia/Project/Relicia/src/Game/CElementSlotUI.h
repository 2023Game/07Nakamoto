#pragma once
#include "CUIBase.h"
#include "CrystalData.h"

class CImage;
class CElementManager;

class CElementSlotUI : public CUIBase
{
public:
	// コンストラクタ
	CElementSlotUI();
	// デストラクタ
	~CElementSlotUI();

	// 指定した属性を属性スロットに装備
	void EquipElement(const CrystalData* data);

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
	CImage* mpElementSlot;	// 属性スロット枠のイメージ
	CImage* mpElementImage;	// 装備しているアイテムのイメージ
};