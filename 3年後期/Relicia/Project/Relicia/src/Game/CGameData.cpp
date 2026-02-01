#include "CGameData.h"
#include "ItemData.h"
#include "CrystalData.h"

int CGameData::floorNum = 1;
std::vector<SlotData> CGameData::ItemSlots;
int CGameData::playerMaxHp = MAX_PLAYER_HP;
int CGameData::playerHp = MAX_PLAYER_HP;
std::vector<CrystalSlot> CGameData::crystalSlots;

// ÉäÉZÉbÉgèàóù
void CGameData::Reset()
{
	floorNum = 1;

	ItemSlots.clear();
	ItemSlots.resize(SLOT_COUNT);
	
	playerMaxHp = MAX_PLAYER_HP;
	playerHp = playerMaxHp;

	crystalSlots.clear();
	crystalSlots.resize(MAX_CRYSTAL_SLOT);
}
