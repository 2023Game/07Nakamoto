#pragma once

// �A�C�e���̎��
enum class ItemType
{
	None = -1,

	Choco,		// �`���R���[�g
	Cookie,		// �N�b�L�[

	Stone,		// ����

	Num,
};

// �A�C�e���̌��ʂ̎��
enum class ItemEffectType
{
	None = -1,		

	RecoveryHP,		// HP��

	Weapon,			// ����

	Throw,			// �����A�C�e��

	Num,
};

struct ItemData
{
	std::string name;		// �A�C�e����
	std::string desc;		// �A�C�e���̐�����

	std::string iconPath;	// �A�C�e���摜�̃p�X
	std::string modelPath;	// ���f���f�[�^�̃p�X

	ItemEffectType effectType;	// �A�C�e���̌��ʂ̎��
	int recovery;		// �񕜗�
	int attack;			// �U����
};

namespace Item
{
	// �A�C�e���̎�ނ��w�肵�āA�A�C�e���̃f�[�^���擾
	bool GetItemData(ItemType type, const ItemData** data);

	// �S�ẴA�C�e���̃��\�[�X��ǂݍ���
	void LoadItemResources();
	// �S�ẴA�C�e���̃��\�[�X��j��
	void DeleteItemResources();
}