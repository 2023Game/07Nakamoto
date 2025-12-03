#pragma once
#include "CrystalData.h"
#include "CTask.h"

#define MAX_SLOT 5

// 属性ストック管理クラス
class CElementManager : public CTask
{
public:
	// インスタンス
	static CElementManager* Instance();
	// コンストラクタ
	CElementManager();
	// デストラクタ
	~CElementManager();

	// 属性を追加する
	void AddElement(ElementType type);

	// 現在選択中の属性を取得
	const CrystalData* GetCurrentElement() const;

	// 属性を消費
	void ConsumeCurentElement();

	// マウスホイールで次へ切り替え
	void SelectNext();
	// マウスホイールで前へ切り替え
	void SelectPrev();

	// 現在のスロットの番号を取得
	int GetCurrentIndex() const;

	// 装備されている属性を使用する
	void UseElement();

	// 更新処理
	void Update() override;
	// 更新処理
	void Render() override;

private:
	static CElementManager* spInstance;

	// 属性スロット
	std::vector<const CrystalData*> mpSlots;
	// 選択中の数値
	int mCurrentIndex;

};