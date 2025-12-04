#pragma once
#include "CrystalData.h"
#include "CTask.h"

#define MAX_SLOT 3
#define MAX_ENERGY 100.0f

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
	// 属性エネルギーの増減
	void AddElementEnergy(ElementType type);
	// 属性エネルギーの減少
	void SubtractElementEnergy(float subtract);

	// 現在選択中の属性を取得
	const CrystalData* GetCurrentElement() const;
	// 指定した属性スロットのデータを取得
	const CrystalData* GetCurrentElementData(int index) const;

	// 現在取得している属性ゲージを取得
	const float GetEnergy(int index);

	// 属性を消費
	//void ConsumeCurentElement();

	// マウスホイールで次へ切り替え
	void SelectNext();
	// マウスホイールで前へ切り替え
	void SelectPrev();

	// 現在のスロットの番号を取得
	int GetCurrentIndex() const;

	// 装備されている属性ゲージがスキルを使うのに足りているか
	bool IsUseElementEnergy();
	// 装備されている属性を使用する
	void UseElement();

	// 更新処理
	void Update() override;
	// 更新処理
	void Render() override;

private:
	struct CrystalSlot
	{
		const CrystalData* data = nullptr;	// クリスタルのデータ
		float currentEnergy = 0.0f;			// 現在のゲージの量
		float maxEnergy = MAX_ENERGY;		// 最大ゲージ量
	};

	static CElementManager* spInstance;

	// 属性スロット
	std::vector<CrystalSlot> mSlots;
	// 選択中の数値
	int mCurrentIndex;

};