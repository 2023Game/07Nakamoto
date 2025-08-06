#pragma once
#include "ItemData.h"

// �A�C�e���̃}�l�[�W���[�N���X
class CItemManager
{
public:
	// �C���X�^���X
	static CItemManager* Instance();
	// �C���X�^���X�̍폜
	static void ClearInstance();

	// �t�B�[���h�̃A�C�e���̐���
	void SpawnItems();

	// �d�͂̌��̐���
	void SpawnDemonPowers();
	// �����𐶐�
	void SpawnCharm(ItemType type, int num);

private:
	// �R���X�g���N�^
	CItemManager();
	// �f�X�g���N�^
	~CItemManager();

	static CItemManager* mpInstance;
	std::vector<CVector> mCharmPositions;	// �A�C�e���̔z�u�̃��X�g
	std::vector<CVector> mDemonPowerPositions;	// �d�͂̌��̔z��̃��X�g
};