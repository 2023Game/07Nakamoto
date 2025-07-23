#pragma once

// �A�C�e���̎��
enum class ItemType
{
	None = -1,

	FlameCharm,		// �Ή��̎���
	BindingCharm,	// �����̎���
	BarrierCharm,	// ���E�̎���
	HealingCharm,	// �����̎���
	SubstituteCharm,// �g����̎���

	Key,			// ��

	Num,
};

// �A�C�e���̌��ʂ̎��
enum class ItemEffectType
{
	None = -1,

	RecoveryHP,		// HP��
	Weapon,			// ����
	Throw,			// �����A�C�e��
	Installation,	// �ݒu�A�C�e��
	Substitute,		// �g����A�C�e��

	Num,
};

// �A�C�e���̃f�[�^
struct ItemData
{
	ItemType type;			// �A�C�e���̎��
	int slotLimit;			// �A�C�e���X���b�g�ɓ�����

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