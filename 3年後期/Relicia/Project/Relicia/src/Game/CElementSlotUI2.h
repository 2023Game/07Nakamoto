#pragma once
#include "CUIBase.h"
#include "CrystalData.h"

class CImage;
class CElementManager;

static const float LARGE = 1.2f;
static const float SMALL = 0.85f;

// 属性UIクラス
class CElementSlotUI2 : public CUIBase
{
public:
	// インスタンス
	//static CElementSlotUI2* Instance();

	// コンストラクタ
	CElementSlotUI2();
	// デストラクタ
	~CElementSlotUI2();

	// 指定したスロットに属性アイコンを設定
	void SetElement(int index, const CrystalData* data);
	// 属性アイコンの画像を生成
	void CreateIcon(CVector2 pos);
	// 属性アイコンの設定
	void RenderIcon(int index, CVector2 pos, float scale, float buttom);
	// 属性アイコンの背景を設定
	void RenderBackIcon(int index, CVector2 pos, float scale);

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
	static CElementSlotUI2* spInstance;

	CImage* mpElementSlot;	// 属性スロット枠のイメージ
	std::vector<CImage*> mpElementImages;		// 属性アイコンのイメージリスト
	std::vector<CImage*> mpElementBackImages;	// 属性アイコンの背景のイメージリスト

};