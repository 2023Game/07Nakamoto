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
	eMoveCrate,		// 動かせる箱
	eInteractObj, // 調べられるアイテム
	eGround,
	ePlayer,
	eEnemy,
	eInteractSearch, // 調べるオブジェクト探知用
	eAttackCol,
};
// 衝突判定レイヤーの初期化リスト
using Layers = std::initializer_list<ELayer>;
