#include "ItemData.h"
#include <map>
#include "CModel.h"

namespace Item
{
	// アイテムのデータテーブル
	const std::map<ItemType, ItemData> ITEM_DATA =
	{
		// チョコレート
		{
			ItemType::Choco,
			{
				ItemType::Choco,
				3,
				"チョコレート",
				"説明文",
				"Item\\Choco\\choco.png",
				"Item\\Choco\\chocolate.obj",
				ItemEffectType::RecoveryHP,
				100,
				0
			}
		},
		// クッキー
		{
			ItemType::Cookie,
			{
				ItemType::Cookie,
				5,
				"クッキー",
				"説明文",
				"Item\\Cookie\\cookie.png",
				"Item\\Cookie\\cookie.obj",
				ItemEffectType::RecoveryHP,
				50,
				0
			}
		},
			// 小石
			{
				ItemType::Stone,
				{
					ItemType::Stone,
					1,
					"小石",
					"説明文＿小石",
					"Item\\Stone\\stone.png",
					"Item\\Stone\\stone.obj",
					ItemEffectType::Throw,
					0,
					0
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
			CResourceManager::Load<CModel>(data->iconPath, data->iconPath, true);
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