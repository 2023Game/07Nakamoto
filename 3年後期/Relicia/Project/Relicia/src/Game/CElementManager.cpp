#include "CElementManager.h"
#include "CrystalData.h"
#include "CAdventurer.h"

CElementManager* CElementManager::spInstance = nullptr;

// インスタンスの生成
CElementManager* CElementManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CElementManager();
	}
	return spInstance;
}

// コンストラクタ
CElementManager::CElementManager()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mCurrentIndex(0)
{
	// スロットの数を設定
	// 初期化
	mSlots.resize(MAX_SLOT);
	
	// 一旦3つの属性を設定
	// TODO:インベントリで変更できるようにする
	AddElement(ElementType::Fire);
	AddElement(ElementType::Water);
	AddElement(ElementType::Thunder);

}

// デストラクタ
CElementManager::~CElementManager()
{
	mSlots.clear();
}

// 属性を追加する
void CElementManager::AddElement(ElementType type)
{
	// 属性データを取得できなかったら、追加できない
	const CrystalData* crystalData = nullptr;
	bool success = Crystal::GetCrystalData(type, &crystalData);
	if (!success) return ;

	bool isAdded = false;

	// 属性スロットのリストの先頭から空のスロットを探して、
	// 空の属性スロットに属性を入れる
	for (auto& slot : mSlots)
	{
		// 空いているスロットを探す
		if (slot.data != nullptr) continue;
			
		slot.data = crystalData;
		slot.currentEnergy = 0.0f;
		slot.maxEnergy = 100.0f;	// 仮の最大値
		isAdded = true;

		break;
	}

	// 属性スロットが満杯の場合、
	if (!isAdded)
	{
		// TODO:満杯の時の処理
	}
}

// 現在選択中の属性を取得
const CrystalData* CElementManager::GetCurrentElement() const
{
	return mSlots[mCurrentIndex].data;
}

// 指定した属性スロットのデータを取得
const CrystalData* CElementManager::GetCurrentElementData(int index) const
{
	return mSlots[index].data;
}

const float CElementManager::GetEnergy(int index)
{
	return mSlots[index].currentEnergy;
}

// 属性を消費
void CElementManager::ConsumeCurentElement()
{
	if (mSlots.empty()) return;

	mSlots[mCurrentIndex].data = nullptr;
	mSlots[mCurrentIndex].currentEnergy = 0.0f;
}

// マウスホイールで次へ切り替え
void CElementManager::SelectNext()
{
	if (mSlots.empty()) return;
	mCurrentIndex = (mCurrentIndex + 1) % mSlots.size();
}

// マウスホイールで前へ切り替え
void CElementManager::SelectPrev()
{
	if (mSlots.empty()) return;
	mCurrentIndex = (mCurrentIndex - 1 + mSlots.size()) % mSlots.size();
}

// 現在のスロットの番号を取得
int CElementManager::GetCurrentIndex() const
{
	return mCurrentIndex;
}

// 装備されている属性を使用する
void CElementManager::UseElement()
{
	

}

// 更新処理
void CElementManager::Update()
{
}

// 更新処理
void CElementManager::Render()
{
}
