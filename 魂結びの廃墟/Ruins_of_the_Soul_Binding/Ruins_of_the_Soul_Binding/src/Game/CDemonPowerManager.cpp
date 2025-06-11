#include "CDemonPowerManager.h"
#include "CDemonPower.h"
#include <assert.h>
#include "CBoss.h"

#define RESERVED_CAPACITYE 7	// ���X�g�̏����e��

// �d�͂̊Ǘ��N���X�̃C���X�^���X
CDemonPowerManager* CDemonPowerManager::spInstance = nullptr;

// �d�͂̌��̃N���X�̃C���X�^���X���擾
CDemonPowerManager* CDemonPowerManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CDemonPowerManager::CDemonPowerManager()
	: CTask(ETaskPriority::eEnemy, 0, ETaskPauseType::eGame)
{
	assert(spInstance == nullptr);
	spInstance = this;

	// ���X�g�̏����e�ʂ̐ݒ�
	mDemonPowers.reserve(RESERVED_CAPACITYE);
}

// �f�X�g���N�^
CDemonPowerManager::~CDemonPowerManager()
{
	spInstance = nullptr;
	mDemonPowers.clear();
}

// �d�͂̌��̒ǉ�
void CDemonPowerManager::AddDemonPower(CDemonPower* power)
{
	mDemonPowers.push_back(power);
}

// �d�͂̌�����菜��
void CDemonPowerManager::RemovePower(CDemonPower* power)
{
	auto find = std::find(mDemonPowers.begin(), mDemonPowers.end(), power);
	if (find == mDemonPowers.end()) return;
	mDemonPowers.erase(find);
}

// �d�͂̌��̐����擾
int CDemonPowerManager::GetDemonPower()
{
	return mDemonPowers.size();
}