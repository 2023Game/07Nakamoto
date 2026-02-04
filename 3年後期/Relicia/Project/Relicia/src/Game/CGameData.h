#pragma once

#define INITIAL_SLOT_ROW 4			// 初期のアイテムスロットの縦の数
#define INITIAL_SLOT_COL 5			// 初期のアイテムスロットの横の数
// アイテムスロット数
#define SLOT_COUNT (INITIAL_SLOT_ROW * INITIAL_SLOT_COL)

#define MAX_PLAYER_HP 20	// 体力の最大値

#define MAX_CRYSTAL_SLOT 3
#define MAX_CRYSTAL_ENERGY 100.0f

struct ItemData;
struct CrystalData;

// アイテムスロットのデータ
struct SlotData
{
	// そのスロットに入っているアイテムのデータ
	const ItemData* data;
	int count;		// 入っているアイテムの個数
	SlotData()
		: data(nullptr)
	{}
};

struct CrystalSlot
{
	const CrystalData* data = nullptr;	// クリスタルのデータ
	float currentEnergy = 0.0f;			// 現在のゲージの量
	float maxEnergy = MAX_CRYSTAL_ENERGY;		// 最大ゲージ量
};

// ゲーム全体で共有するデータ
class CGameData
{
public:
	// リセット処理
	static void Reset();

	static int floorNum;	// 現在の階層
	// アイテムスロットのリスト
	static std::vector<SlotData> ItemSlots;	
	
	static int playerMaxHp;		// プレイヤーの最大HP
	static int playerHp;		// プレイヤーの現在HP

	// 属性スロット
	static std::vector<CrystalSlot> crystalSlots;

};