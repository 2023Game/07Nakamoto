#pragma once
#include "CTexture.h"
#include "CUIBase.h"

class CImage;

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

	CImage* mpEquipmentSlot;	// 装備スロット枠のイメージ
};