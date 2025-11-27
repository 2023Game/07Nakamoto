#include "CrystalData.h"
#include <map>
#include "CModel.h"

#define ICON_PATH "UI\\Element\\"

namespace Crystal
{
	// クリスタルのデータテーブル
	const std::map<ElementType, CrystalData> CRYSTAL_DATA =
	{
		// 炎のクリスタル
		{
			ElementType::Fire,
			{
				ElementType::Fire,
				ICON_PATH"fire_icon.png",
				"Field\\Object\\key.obj" //仮
			}
		},

		// 水のクリスタル
		{
			ElementType::Water,
			{
				ElementType::Water,
				ICON_PATH"water_icon.png",
				"Field\\Object\\key.obj" //仮
			}
		},
	};

	// クリスタルのリソースを既に読み込んでいるかどうか
	bool loadedCrystalResources = false;

	// クリスタルの種類を指定して、クリスタルのデータ取得
	bool GetCrystalData(ElementType type, const CrystalData** data)
	{
		// テーブルから指定された種類のクリスタルデータを探す
		auto find = CRYSTAL_DATA.find(type);

		// クリスタルデータが存在しなかった
		if (find == CRYSTAL_DATA.end())
		{
			// nullptrで空にしてやる
			*data = nullptr;
			return false;
		}

		// 見つかったクリスタルデータのアドレスを返す
		*data = &find->second;
		return true;
	}

	// 全てのクリスタルのリソースを読み込む
	void LoadResources()
	{
		// 既にクリスタルのリソースが読み込まれていたら、再度読み込みしなおさない
		if (loadedCrystalResources) return;

		int count = (int)ElementType::Num;
		for (int i = 0; i < count; i++)
		{
			ElementType type = (ElementType)i;
			const CrystalData* data = nullptr;
			// クリスタルデータ取得
			if (!GetCrystalData(type, &data)) continue;

			// クリスタルのアイコン画像とモデルデータを読み込む
			CResourceManager::Load<CTexture>(data->iconPath, data->iconPath, true);
			CResourceManager::Load<CModel>(data->modelPath, data->modelPath, true);
		}

		// クリスタルのリソース読み込みフラグを立てる
		loadedCrystalResources = true;
	}

	// 全てのクリスタルのリソースの破棄
	void DeleteResources()
	{
		// クリスタルのリソースが読み込まれてなければ、スルー
		if (!loadedCrystalResources) return;

		int count = (int)ElementType::Num;
		for (int i = 0; i < count; i++)
		{
			ElementType type = (ElementType)i;
			const CrystalData* data = nullptr;
			// クリスタルデータ取得
			if (!GetCrystalData(type, &data)) continue;

			// クリスタルのアイコン画像とモデルデータを削除
			CResourceManager::Delete(data->iconPath);
			CResourceManager::Delete(data->modelPath);
		}

		// クリスタルのリソースを破棄したので、フラグも初期化
		loadedCrystalResources = false;
	}
}