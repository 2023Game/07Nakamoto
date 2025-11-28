#include "CElementManager.h"
#include "CrystalData.h"
#include "CAdventurer.h"

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
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mCurrentIndex(0)
{
	// スロットの数を設定
	// 初期化
	mpSlots.resize(MAX_SLOT, nullptr);

}

// デストラクタ
CElementManager::~CElementManager()
{
	mpSlots.clear();
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
	for (auto& slot : mpSlots)
	{
		// 空いているスロットを探す
		if (slot != nullptr) continue;
			
		slot = crystalData;
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
	return mpSlots[mCurrentIndex];
}

// 属性を消費
void CElementManager::ConsumeCurentElement()
{
	if (mpSlots.empty()) return;

	mpSlots[mCurrentIndex] = nullptr;
}

// マウスホイールで次へ切り替え
void CElementManager::SelectNext()
{
	if (mpSlots.empty()) return;
	mCurrentIndex = (mCurrentIndex + 1) % mpSlots.size();
}

// マウスホイールで前へ切り替え
void CElementManager::SelectPrev()
{
	if (mpSlots.empty()) return;
	mCurrentIndex = (mCurrentIndex - 1 + mpSlots.size()) % mpSlots.size();
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

// 更新処理
void CElementManager::Update()
{
	CAdventurer::Instance()->EquipElement(mCurrentIndex);
}

// 更新処理
void CElementManager::Render()
{
}
