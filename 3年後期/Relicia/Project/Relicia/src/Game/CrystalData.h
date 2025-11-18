#pragma once

// クリスタルの種類
enum class CrystalType
{
	None = -1,

	Fire,		// 炎
	Water,		// 水
	Thunder,	// 雷

	Num
};

// クリスタルのデータ
struct CrystalData
{
	CrystalType type;	// クリスタルの種類

	std::string iconPath;	// クリスタルの画像のパス
	std::string modelPath;	// モデルデータのパス
};

namespace Crystal
{
	// クリスタルの種類を指定して、クリスタルのデータ取得
	bool GetCrystalData(CrystalType type, const CrystalData** data);

	// 全てのクリスタルのリソースを読み込む
	void LoadResources();

	// 全てのクリスタルのリソースの破棄
	void DeleteResources();
}

