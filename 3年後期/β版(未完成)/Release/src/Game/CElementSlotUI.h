#pragma once
#include "CUIBase.h"
#include "CrystalData.h"

class CImage;
class CElementManager;

class CElementSlotUI : public CUIBase
{
public:
	// インスタンス
	static CElementSlotUI* Instance();

	// コンストラクタ
	CElementSlotUI();
	// デストラクタ
	~CElementSlotUI();

	// 指定したスロットに属性アイコンを設定
	void SetElement(int index, const CrystalData* data);

	// 指定した属性を属性スロットに装備
	void EquipElement(const CrystalData* data);

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
	static CElementSlotUI* spInstance;

	CImage* mpElementSlot;	// 属性スロット枠のイメージ
	std::vector<CImage*> mpElementImages;	// 属性アイコンのイメージリスト
};