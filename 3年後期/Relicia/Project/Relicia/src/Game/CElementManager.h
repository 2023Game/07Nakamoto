#pragma once

// 属性ストック管理クラス
class CElementManager
{
public:
	// インスタンス
	static CElementManager* Instance();
	// コンストラクタ
	CElementManager();
	// デストラクタ
	~CElementManager();

	// 属性タイプ
	enum class ElementType
	{
		None = -1,

		eFire,		// 炎属性
		eWater,		// 水属性
		eThunder,	// 雷属性
		eIce,		// 氷属性
		eWind,		// 風属性

	};

	// 属性を追加する
	bool AddElement(ElementType type);

	// 現在選択中の属性を取得
	ElementType GetCurrentElement() const;

	// 属性を消費
	void ConsumeCurentElement();

	// マウスホイールで次へ切り替え
	void SelectNext();
	// マウスホイールで前へ切り替え
	void SelectPrev();

	// スロット情報取得
	const std::vector<ElementType>& GetSlots() const;
	// 現在のスロットの番号を取得
	int GetCurrentIndex() const;

private:
	static CElementManager* spInstance;

	// 属性スロット
	std::vector<ElementType> mSlots;
	// 選択中の数値
	int mCurrentIndex;
};