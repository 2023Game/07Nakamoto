#include "ItemData.h"
#include <map>
#include "CModel.h"
#include "Maths.h"

#define ICON_SIZE 64	// アイコンのサイズ
#define ICON_PADDING 2	// アイコン同士の隙間
#define ICON_START_X 2	// 最初のアイコンのX座標
#define ICON_START_Y 2	// 最初のアイコンのY座標

namespace Item
{
	// アイテムのデータテーブル
	const std::map<ItemId, ItemData> ITEM_DATA =
	{
		// 回復薬
		{
			ItemId::HealingPotion,
			{
				ItemId::HealingPotion,
				5,
				"回復薬(小)",
				"説明文",
				"",
				ItemEffectType::RecoveryHP,
				50,
				0,
				300,
				100,
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
		// 回復薬
		{
			ItemId::HealingPotion2,
			{
				ItemId::HealingPotion2,
				5,
				"回復薬(中)",
				"説明文",
				"",
				ItemEffectType::RecoveryHP,
				50,
				0,
				500,
				200,
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
		// 回復薬
		{
			ItemId::HealingPotion3,
			{
				ItemId::HealingPotion3,
				5,
				"回復薬(大)",
				"説明文",
				"",
				ItemEffectType::RecoveryHP,
				50,
				0,
				1000,
				500,
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
		// 爆弾
		{
			ItemId::Bomb,
			{
				ItemId::Bomb,
				3,
				"爆弾",
				"説明文",
				"",
				ItemEffectType::RecoveryHP,
				30,
				0,
				1300,
				800,
			}
		},
		// パン
		{
			ItemId::Bread,
			{
				ItemId::Bread,
				3,
				"パン",
				"説明文",
				"",
				ItemEffectType::RecoveryHP,
				30,
				0,
				150,
				50,
			}
		},
		// チキン
		{
			ItemId::Chicken,
			{
				ItemId::Chicken,
				3,
				"チキン",
				"説明文",
				"",
				ItemEffectType::RecoveryHP,
				30,
				0,
				1000,
				600,
			}
		},
		// 鍵
		{
			ItemId::Key,
			{
				ItemId::Key,
				1,
				"鍵",
				"説明文",
				"",
				ItemEffectType::Key,
				0,
				0,
				1000,
				10,
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
		// 鍵
		{
			ItemId::Key2,
			{
				ItemId::Key2,
				1,
				"鍵",
				"説明文",
				"",
				ItemEffectType::Key,
				0,
				0,
				1000,
				10,
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
		// 鍵
		{
			ItemId::Key3,
			{
				ItemId::Key3,
				1,
				"鍵",
				"説明文",
				"",
				ItemEffectType::Key,
				0,
				0,
				1000,
				10,
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
	};

	// アイコンのリソースを既に読み込んでいるかどうか
	bool loadedItemResources = false;

	// アイテムの種類を指定して、アイテムのデータを取得
	bool GetItemData(ItemId type, const ItemData** data)
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

	// ランダムにItemIdを返す
	ItemId GetRandomItemId()
	{
		int rd = Math::Rand(0, (int)ItemId::Num - 1);

		return static_cast<ItemId>(rd);
	}

	// 全てのアイテムのリソースを読み込む
	void LoadItemResources()
	{
		// 既にアイテムのリソースが読み込まれていたら、再度読み込みしなおさない
		if (loadedItemResources) return;

		CResourceManager::Load<CTexture>("ItemSpreadSheet", "Item\\2D\\spreadsheet.png");

		int count = (int)ItemId::Num;
		for (int i = 0; i < count; i++)
		{
			ItemId type = (ItemId)i;
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

		int count = (int)ItemId::Num;
		for (int i = 0; i < count; i++)
		{
			ItemId type = (ItemId)i;
			const ItemData* data = nullptr;
			// アイテムデータ取得
			if (!GetItemData(type, &data)) continue;

			CResourceManager::Delete(data->modelPath);
		}

		// アイテムのリソースを破棄したので、フラグも初期化
		loadedItemResources = false;
	}

	CRect CalcIconUV(ItemId id, const CVector2& texSize)
	{
		CRect r;

		float iconSpace = ICON_SIZE + ICON_PADDING * 2.0f;

		int countW = (int)(texSize.X() / iconSpace);

		int x, y;
		x = (int)id % countW;
		y = (int)id / countW;

		r.X((ICON_PADDING + iconSpace * x) / texSize.X());
		r.Y((texSize.Y() - (iconSpace * (y + 1) - ICON_PADDING)) / texSize.Y());
		r.W(ICON_SIZE / texSize.X());
		r.H(ICON_SIZE / texSize.Y());

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