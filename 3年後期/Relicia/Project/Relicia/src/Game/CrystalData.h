#pragma once

// 属性の種類
enum class ElementType
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
	ElementType type;	// 属性の種類

	std::string iconPath;	// クリスタルの画像のパス
	std::string modelPath;	// モデルデータのパス
};

namespace Crystal
{
	// 属性の種類を指定して、属性のデータ取得
	bool GetCrystalData(ElementType type, const CrystalData** data);

	// 全てのクリスタルのリソースを読み込む
	void LoadResources();

	// 全てのクリスタルのリソースの破棄
	void DeleteResources();
}

