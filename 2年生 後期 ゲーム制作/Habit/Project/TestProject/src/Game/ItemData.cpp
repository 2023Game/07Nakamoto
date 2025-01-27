#include "ItemData.h"
#include <map>
#include "CModel.h"

namespace Item
{
	// �A�C�e���̃f�[�^�e�[�u��
	const std::map<ItemType, ItemData> ITEM_DATA =
	{
		// �`���R���[�g
		{
			ItemType::Choco,
			{
				ItemType::Choco,
				3,
				"�`���R���[�g",
				"������",
				"Item\\Choco\\choco.png",
				"Item\\Choco\\chocolate.obj",
				ItemEffectType::RecoveryHP,
				100,
				0
			}
		},
		// �N�b�L�[
		{
			ItemType::Cookie,
			{
				ItemType::Cookie,
				5,
				"�N�b�L�[",
				"������",
				"Item\\Cookie\\cookie.png",
				"Item\\Cookie\\cookie.obj",
				ItemEffectType::RecoveryHP,
				50,
				0
			}
		},
			// ����
			{
				ItemType::Stone,
				{
					ItemType::Stone,
					1,
					"����",
					"�������Q����",
					"Item\\Stone\\stone.png",
					"Item\\Stone\\stone.obj",
					ItemEffectType::Throw,
					0,
					0
				}
			},

	};

	// �A�C�R���̃��\�[�X�����ɓǂݍ���ł��邩�ǂ���
	bool loadedItemResources = false;

	// �A�C�e���̎�ނ��w�肵�āA�A�C�e���̃f�[�^���擾
	bool GetItemData(ItemType type, const ItemData** data)
	{
		// �e�[�u������w�肳�ꂽ��ނ̃A�C�e���f�[�^��T��
		auto find = ITEM_DATA.find(type);

		// �A�C�e���f�[�^�����݂��Ȃ�����
		if (find == ITEM_DATA.end())
		{
			// nullptr�ŋ�ɂ��Ă��
			*data = nullptr;
			return false;
		}

		// ���������A�C�e���f�[�^�̃A�h���X��Ԃ�
		*data = &find->second;
		return true;
	}

	// �S�ẴA�C�e���̃��\�[�X��ǂݍ���
	void LoadItemResources()
	{
		// ���ɃA�C�e���̃��\�[�X���ǂݍ��܂�Ă�����A�ēx�ǂݍ��݂��Ȃ����Ȃ�
		if (loadedItemResources) return;

		int count = (int)ItemType::Num;
		for (int i = 0; i < count; i++)
		{
			ItemType type = (ItemType)i;
			const ItemData* data = nullptr;
			// �A�C�e���f�[�^�擾
			if (!GetItemData(type, &data)) continue;

			// �A�C�e���̃A�C�R���摜�ƃ��f���f�[�^��ǂݍ���
			CResourceManager::Load<CModel>(data->iconPath, data->iconPath, true);
			CResourceManager::Load<CModel>(data->modelPath, data->modelPath, true);
		}

		// �A�C�e���̃��\�[�X�ǂݍ��݃t���O�𗧂Ă�
		loadedItemResources = true;
	}

	// �S�ẴA�C�e���̃��\�[�X��j��
	void DeleteItemResources()
	{
		// �A�C�e���̃��\�[�X���ǂݍ��܂�ĂȂ���΁A�X���[
		if (!loadedItemResources) return;

		int count = (int)ItemType::Num;
		for (int i = 0; i < count; i++)
		{
			ItemType type = (ItemType)i;
			const ItemData* data = nullptr;
			// �A�C�e���f�[�^�擾
			if (!GetItemData(type, &data)) continue;

			// �A�C�e���̃A�C�R���摜�ƃ��f���f�[�^���폜
			CResourceManager::Delete(data->iconPath);
			CResourceManager::Delete(data->modelPath);
		}

		// �A�C�e���̃��\�[�X��j�������̂ŁA�t���O��������
		loadedItemResources = false;
	}
}