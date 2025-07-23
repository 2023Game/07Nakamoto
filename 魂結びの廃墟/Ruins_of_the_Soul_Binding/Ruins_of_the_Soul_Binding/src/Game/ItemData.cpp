#include "ItemData.h"
#include <map>
#include "CModel.h"

namespace Item
{
	// アイテムのデータテーブル
	const std::map<ItemType, ItemData> ITEM_DATA =
	{
		// 火炎の呪符
		{
			ItemType::FlameCharm,
			{
				ItemType::FlameCharm,
				5,
				"火炎の呪符",
				"説明文",
				"Item\\2D\\flameCharm.png",
				"Item\\3D\\flameCharm.obj",
				ItemEffectType::Throw,
				0,
				20
			}
		},
		// 束縛の呪符
		{
			ItemType::BindingCharm,
			{
				ItemType::BindingCharm,
				5,
				"束縛の呪符",
				"説明文",
				"Item\\2D\\bindingCharm.png",
				"Item\\3D\\bindingCharm.obj",
				ItemEffectType::Throw,
				0,
				0
			}
		},
		// 結界の呪符
		{
			ItemType::BarrierCharm,
			{
				ItemType::BarrierCharm,
				5,
				"結界の呪符",
				"説明文",
				"Item\\2D\\barrierCharm.png",
				"Item\\3D\\barrierCharm.obj",
				ItemEffectType::Installation,
				0,
				0
			}
		},
		// 治癒の呪符
		{
			ItemType::HealingCharm,
			{
				ItemType::HealingCharm,
				5,
				"治癒の呪符",
				"説明文",
				"Item\\2D\\healingCharm.png",
				"Item\\3D\\healingCharm.obj",
				ItemEffectType::RecoveryHP,
				0,
				20
			}
		},
		// 身代わりの呪符
		{
			ItemType::SubstituteCharm,
			{
				ItemType::SubstituteCharm,
				5,
				"身代わりの呪符",
				"説明文",
				"Item\\2D\\substituteCharm.png",
				"Item\\3D\\substituteCharm.obj",
				ItemEffectType::Throw,
				0,
				20
			}
		},
	};

	// アイコンのリソースを既に読み込んでいるかどうか
	bool loadedItemResources = false;

	// アイテムの種類を指定して、アイテムのデータを取得
	bool GetItemData(ItemType type, const ItemData** data)
	{
		// テーブルから指定された種類のアイテムデータを探す
		auto find = ITEM_DATA.find(type);

		// アイテムデータが存在しなかった
		if (find == ITEM_DATA.end())
		{
			// nullptrで空にしてやる
			*data = nullptr;
			return false;
		}

		// 見つかったアイテムデータのアドレスを返す
		*data = &find->second;
		return true;
	}

	// 全てのアイテムのリソースを読み込む
	void LoadItemResources()
	{
		// 既にアイテムのリソースが読み込まれていたら、再度読み込みしなおさない
		if (loadedItemResources) return;

		int count = (int)ItemType::Num;
		for (int i = 0; i < count; i++)
		{
			ItemType type = (ItemType)i;
			const ItemData* data = nullptr;
			// アイテムデータ取得
			if (!GetItemData(type, &data)) continue;

			// アイテムのアイコン画像とモデルデータを読み込む
			CResourceManager::Load<CTexture>(data->iconPath, data->iconPath, true);
			CResourceManager::Load<CModel>(data->modelPath, data->modelPath, true);
		}

		// アイテムのリソース読み込みフラグを立てる
		loadedItemResources = true;
	}

	// 全てのアイテムのリソースを破棄
	void DeleteItemResources()
	{
		// アイテムのリソースが読み込まれてなければ、スルー
		if (!loadedItemResources) return;

		int count = (int)ItemType::Num;
		for (int i = 0; i < count; i++)
		{
			ItemType type = (ItemType)i;
			const ItemData* data = nullptr;
			// アイテムデータ取得
			if (!GetItemData(type, &data)) continue;

			// アイテムのアイコン画像とモデルデータを削除
			CResourceManager::Delete(data->iconPath);
			CResourceManager::Delete(data->modelPath);
		}

		// アイテムのリソースを破棄したので、フラグも初期化
		loadedItemResources = false;
	}

}