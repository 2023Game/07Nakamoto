#include "CElementManager.h"
#include "CrystalData.h"

#define MAX_SLOT 5

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
	: mCurrentIndex(0)
{
	// スロットの数を設定
	// 初期化
	mSlots.resize(MAX_SLOT,ElementType::None);

	//mSlots[0] = ElementType::eFire;
	//mSlots[3] = ElementType::eWater;

}

// デストラクタ
CElementManager::~CElementManager()
{
	mSlots.clear();
}

// 属性を追加する
bool CElementManager::AddElement(ElementType type)
{
	// 属性データを取得できなかったら、追加できない
	const CrystalData* crystalData = nullptr;
	bool success = Crystal::GetCrystalData(type, &crystalData);
	if (!success) return false;

	// 属性スロットのリストの先頭から空のスロットを探して、
	// 空の属性スロットに属性を入れる
	for (auto& slot : mSlots)
	{
		// 空いているスロットを探す
		if (slot == ElementType::None)
		{
			slot = type;
			return true;
		}
	}

	return false;	// 満タン
}

// 現在選択中の属性を取得
//CElementManager::Elem CElementManager::GetCurrentElement() const
//{
//	return mSlots[mCurrentIndex];
//}

// 属性を消費
void CElementManager::ConsumeCurentElement()
{
	if (mSlots.empty()) return;

	mSlots[mCurrentIndex] = ElementType::None;
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

// スロット情報取得
//const std::vector<CElementManager::ElementType>& CElementManager::GetSlots() const
//{
//	return mSlots;
//}

// 現在のスロットの番号を取得
int CElementManager::GetCurrentIndex() const
{
	return mCurrentIndex;
}