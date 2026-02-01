#include "CElementManager.h"
#include "CrystalData.h"
#include "CAdventurer.h"
#include "CGameData.h"

#define TRUE_ADD_ENERGY 20.0f	// 同じ属性の時のエネルギーの増加量
#define FALSE_ADD_ENERGY 10.0f	// 異なる属性の時のエネルギーの増加量

#define ENERGY_CONSUMPTION 30.0f	// 属性エネルギーの消費量

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
	// 初期化
	
	// 一旦3つの属性を設定
	// TODO:インベントリで変更できるようにする
	AddElement(ElementType::Fire);
	AddElement(ElementType::Water);
	AddElement(ElementType::Thunder);

}

// デストラクタ
CElementManager::~CElementManager()
{
	// 破棄されたら、インスタンス変数を空にする
	if (spInstance == this)
	{
		spInstance = nullptr;
	}
}

// 属性を追加する
void CElementManager::AddElement(ElementType type)
{
	// 属性データを取得できなかったら、追加できない
	const CrystalData* crystalData = nullptr;
	bool success = Crystal::GetCrystalData(type, &crystalData);
	if (!success) return ;

	bool isAdded = false;

	// 追加する属性のスロットが既にある場合は追加しない
	for (auto& slot : CGameData::crystalSlots)
	{
		// 空いているスロットを探す
		if (slot.data == crystalData) return;
	}

	// 属性スロットのリストの先頭から空のスロットを探して、
	// 空の属性スロットに属性を入れる
	for (auto& slot : CGameData::crystalSlots)
	{
		// 空いているスロットを探す
		if (slot.data != nullptr) continue;
			
		slot.data = crystalData;
		slot.maxEnergy = MAX_CRYSTAL_ENERGY;
		isAdded = true;

		break;
	}

	//// 属性スロットが満杯の場合、
	//if (!isAdded)
	//{
	//	// 加算しない
	//}
}

// 属性エネルギーを加算
void CElementManager::AddElementEnergy(ElementType type)
{
	for (auto& slot : CGameData::crystalSlots)
	{
		// 属性スロットが空か確認
		if (slot.data == nullptr) continue;

		// スロットの属性と同じ場合、
		if (slot.data->type == type)
		{
			slot.currentEnergy += TRUE_ADD_ENERGY;
		}
		// スロットの属性と異なる場合
		else
		{
			slot.currentEnergy += FALSE_ADD_ENERGY;
		}

		// エネルギー上限を越えた場合、
		if (slot.currentEnergy > MAX_CRYSTAL_ENERGY)
		{
			slot.currentEnergy = MAX_CRYSTAL_ENERGY;
		}
	}
}

// 属性エネルギーの減少
void CElementManager::SubtractElementEnergy(float subtract)
{
	CGameData::crystalSlots[mCurrentIndex].currentEnergy -= subtract;
}

// 現在装備中の属性を取得
const CrystalData* CElementManager::GetCurrentElement() const
{
	return CGameData::crystalSlots[mCurrentIndex].data;
}

// 指定した属性スロットのデータを取得
const CrystalData* CElementManager::GetCurrentElementData(int index) const
{
	return CGameData::crystalSlots[index].data;
}

// 現在取得している属性ゲージを取得
const float CElementManager::GetEnergy(int index)
{
	return CGameData::crystalSlots[index].currentEnergy;
}

//// 属性を消費
//void CElementManager::ConsumeCurentElement()
//{
//	if (mSlots.empty()) return;
//
//	mSlots[mCurrentIndex].data = nullptr;
//	//mSlots[mCurrentIndex].currentEnergy = 0.0f;
//}

// マウスホイールで次へ切り替え
void CElementManager::SelectNext()
{
	if (CGameData::crystalSlots.empty()) return;
	mCurrentIndex = (mCurrentIndex + 1) % CGameData::crystalSlots.size();
}

// マウスホイールで前へ切り替え
void CElementManager::SelectPrev()
{
	if (CGameData::crystalSlots.empty()) return;
	mCurrentIndex = (mCurrentIndex - 1 + CGameData::crystalSlots.size()) % CGameData::crystalSlots.size();
}

// 現在のスロットの番号を取得
int CElementManager::GetCurrentIndex() const
{
	return mCurrentIndex;
}

// 装備されている属性ゲージがスキルを使うのに足りているか
bool CElementManager::IsUseElementEnergy()
{
	if (CGameData::crystalSlots[mCurrentIndex].currentEnergy > ENERGY_CONSUMPTION) return true;
	
	return false;
}

// 装備されている属性を使用する
void CElementManager::UseElement()
{
	if (IsUseElementEnergy())
	{
		SubtractElementEnergy(ENERGY_CONSUMPTION);
	}

}

// 更新処理
void CElementManager::Update()
{
	//CDebugPrint::Print("1属性ゲージ：%f\n", mSlots[0].currentEnergy);
	//CDebugPrint::Print("2属性ゲージ：%f\n", mSlots[1].currentEnergy);
	//CDebugPrint::Print("3属性ゲージ：%f\n", mSlots[2].currentEnergy);
}

// 更新処理
void CElementManager::Render()
{
}
