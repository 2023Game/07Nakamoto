#include "CItemManager.h"
#include "CDemonPower.h"
#include "CItemObj.h"
#include "Maths.h"

#define DEMONPOWER		5	// �d�͂̌��̐�
#define FLAMECHARM		10	// �Ή��̎���
#define BINDINGCHARM	5	// �����̎���
#define BARRIERCHARM	5	// ���E�̎���
#define HEALINGCHARM	10	// �����̎���
#define SUBSTITUTECHARM	3	// �g����̎���

CItemManager* CItemManager::mpInstance = nullptr;

// �C���X�^���X�̐���
CItemManager* CItemManager::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CItemManager();
	}

	return mpInstance;;
}

// �R���X�g���N�^
CItemManager::CItemManager()
	: mDemonPowerPositions(
		{
			// �d�͂̌��̍��W��ݒ�(18)
			CVector(140.0f, 5.0f, 120.0f),
			CVector(140.0f, 5.0f, -20.0f),
			CVector(180.0f, 5.0f, 380.0f),
			CVector(-30.0f, 5.0f, 220.0f),
			CVector(-200.0f, 5.0f, 120.0f),
			CVector(-100.0f, 5.0f, 430.0f),
			CVector(-130.0f, 5.0f, 245.0f),
			CVector(-130.0f, 5.0f, 195.0f),
			CVector(-150.0f, 5.0f, 0.0f),
			CVector(-200.0f, 5.0f, 75.0f),
			CVector(-250.0f, 5.0f, -265.0f),
			CVector(-80.0f, 5.0f, -265.0f),
			CVector(45.0f, 5.0f, -275.0f),
			CVector(-150.0f, 5.0f, 0.0f),
			CVector(160.0f, 5.0f, 75.0f),
			CVector(-50.0f, 5.0f, 350.0f),
			CVector(-300.0f,5.0f,330.0f),
			CVector(-275.0f,5.0f,90.0f),
			CVector(-30.0f,5.0f,130.0f),
		})
	, mCharmPositions(
		{
			// �A�C�e���̍��W��ݒ�(29)
			CVector(40.0f, 0.5f, -100.0f),
			CVector(40.0f, 0.5f, -210.0f),
			CVector(-85.0f, 0.5f, -260.0f),
			CVector(-180.0f, 0.5f, -260.0f),
			CVector(-205.0f, 0.5f, -280.0f),
			CVector(-180.0f, 0.5f, -260.0f),
			CVector(-315.0f, 0.5f, -280.0f),
			CVector(-315.0f, 0.5f, -75.0f),
			CVector(-240.0f, 0.5f, -75.0f),
			CVector(-200.0f, 0.5f, -25.0f),
			CVector(-200.0f, 0.5f, -155.0f),
			CVector(-100.0f, 0.5f, -125.0f),
			CVector(-100.0f, 0.5f, -20.0f),
			CVector(-180.0f, 0.5f, -260.0f),
			CVector(125.0f, 0.5f, -20.0f),
			CVector(165.0f, 0.5f, -135.0f),
			CVector(195.0f, 0.5f, 250.0f),
			CVector(160.0f, 0.5f, 295.0f),
			CVector(175.0f, 0.5f, 430.0f),
			CVector(145.0f, 0.5f, 325.0f),
			CVector(45.0f, 0.5f, 400.0f),
			CVector(65.0f, 0.5f, 350.0f),
			CVector(-115.0f, 0.5f, 420.0f),
			CVector(-200.0f, 0.5f, 440),
			CVector(-300, 0.5f, 415.0f),
			CVector(-320.0f, 0.5f, 260.0f),
			CVector(-265.0f, 0.5f, 215.0f),
			CVector(-135.0f, 0.5f, 260.0f),
			CVector(-220.0f, 0.5f, 300.0f),
			CVector(-200.0f, 0.5f, 345.0f),
			CVector(-150.0f, 0.5f, 320.0f),
			CVector(-125.0f, 0.5f, 150.0f),
			CVector(-145.0f, 0.5f, 115.0f),
			CVector(-260.0f, 0.5f, 125.0f),
			CVector(-300.0f, 0.5f, 80.0f),
			CVector(-300.0f, 0.5f, 30.0f),
			CVector(-320.0f, 0.5f, 260.0f),
			CVector(-310.0f, 0.5f, -70.0f),
		})
{
}

// �f�X�g���N�^
CItemManager::~CItemManager()
{
	if (mpInstance == this)
	{
		mpInstance = nullptr;
	}
}

// �C���X�^���X�̍폜
void CItemManager::ClearInstance()
{
	SAFE_DELETE(mpInstance);
}

// �t�B�[���h�̃A�C�e���̐���
void CItemManager::SpawnItems()
{
	// �d�͂̌��𐶐�
	SpawnDemonPowers();
	// �Ή��̎����𐶐�
	SpawnCharm(ItemType::FlameCharm, FLAMECHARM);
	// �����̎���
	SpawnCharm(ItemType::HealingCharm, HEALINGCHARM);
}

// �d�͂̌��̐����ꏊ
void CItemManager::SpawnDemonPowers()
{
	std::vector<CVector> positions = mDemonPowerPositions;
	for (int i = 0; i < DEMONPOWER; i++)
	{
		int count = positions.size();
		if (count == 0)break;

		int index = Math::Rand(0, count - 1);
		CVector pos = positions[index];
		// �d�͂̌��̐���
		new CDemonPower(pos);
		positions.erase(positions.begin() + index);
	}
}

// �����𐶐�
void CItemManager::SpawnCharm(ItemType type, int num)
{
	std::vector<CVector> positions = mCharmPositions;
	for (int i = 0; i < num; i++)
	{
		int count = positions.size();
		if (count == 0)break;

		int index = Math::Rand(0, count - 1);
		CVector pos = positions[index];
		// �����̐���
		new CItemObj(type, pos);
		positions.erase(positions.begin() + index);
	}

	mCharmPositions = positions;
}
