#include "ItemData.h"
#include <map>
#include "CModel.h"

#define ICON_SIZE 64	// アイコンのサイズ
#define ICON_PADDING 2	// アイコン同士の隙間
#define ICON_START_X 2	// 最初のアイコンのX座標
#define ICON_START_Y 2	// 最初のアイコンのY座標

namespace Item
{
	// アイテムのデータテーブル
	const std::map<ItemType, ItemData> ITEM_DATA =
	{
		// 回復薬
		{
			ItemType::HealingPotion,
			{
				ItemType::HealingPotion,
				5,
				"回復薬",
				"説明文",
				//"Item\\2D\\healing_potion.png",
				"Item\\2D\\spreadsheet.png",
				{0,0},
				"",
				ItemEffectType::RecoveryHP,
				50,
				0,
				//1,2,
				//{
				//	{1,0,0,0,0},
				//	{1,0,0,0,0},
				//	{0,0,0,0,0},
				//	{0,0,0,0,0},
				//	{0,0,0,0,0}
				//}
			}
		},
		// 鍵
		{
			ItemType::Key,
			{
				ItemType::Key,
				1,
				"鍵",
				"説明文",
				//"Item\\2D\\key.png",
				"Item\\2D\\spreadsheet.png",
				{1,0},
				"",
				ItemEffectType::Key,
				0,
				0,
				//1,1,
				//{
				//	{1,0,0,0,0},
				//	{0,0,0,0,0},
				//	{0,0,0,0,0},
				//	{0,0,0,0,0},
				//	{0,0,0,0,0}
				//}
			}
		},
		// パン
		{
			ItemType::Bread,
			{
				ItemType::Bread,
				3,
				"鍵",
				"説明文",
				//"Item\\2D\\bread",
				"Item\\2D\\spreadsheet.png",
				{2,0},
				"",
				ItemEffectType::RecoveryHP,
				30,
				0,
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

		CResourceManager::Load<CTexture>("ItemSpreadSheet", "Item\\2D\\spreadsheet.png");

		int count = (int)ItemType::Num;
		for (int i = 0; i < count; i++)
		{
			ItemType type = (ItemType)i;
			const ItemData* data = nullptr;
			// アイテムデータ取得
			if (!GetItemData(type, &data)) continue;

			// アイテムのアイコン画像とモデルデータを読み込む
			//CResourceManager::Load<CTexture>(data->iconPath, data->iconPath, true);
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

		// アイテムのアイコン画像とモデルデータを削除
		CResourceManager::Delete("Item\\2D\\spreadsheet.png");

		int count = (int)ItemType::Num;
		for (int i = 0; i < count; i++)
		{
			ItemType type = (ItemType)i;
			const ItemData* data = nullptr;
			// アイテムデータ取得
			if (!GetItemData(type, &data)) continue;

			CResourceManager::Delete(data->modelPath);
		}

		// アイテムのリソースを破棄したので、フラグも初期化
		loadedItemResources = false;
	}

	Rect CalcIconRect(const SpriteIndex& index)
	{
		Rect r;

		r.x = ICON_START_X + index.col * (ICON_SIZE + ICON_PADDING);
		r.y = ICON_START_Y + index.row * (ICON_SIZE + ICON_PADDING);

		r.w = ICON_SIZE;
		r.h = ICON_SIZE;

		return r;
	}

	CVector4 RectToUV(int x, int y, int w, int h, int texW, int texH)
	{
		float u0 = (x + 0.5f) / texW;
		float v0 = (y + 0.5f) / texH;
		float u1 = (x + w - 0.5f) / texW;
		float v1 = (y + h - 0.5f) / texH;

		// (u0, v0, u1, v1)
		return CVector4(u0, v0, u1, v1);
	}

}