#pragma once
#include <initializer_list>

// 衝突判定レイヤー
enum class ELayer
{
	eNone = -1,
	eTest,
	eFloor,
	eWall,
	eCeil,		// 天井
	eCrystal,	// クリスタル
	eGround,
	ePlayer,
	eEnemy,

	eAttackCol,
};
// 衝突判定レイヤーの初期化リスト
using Layers = std::initializer_list<ELayer>;
