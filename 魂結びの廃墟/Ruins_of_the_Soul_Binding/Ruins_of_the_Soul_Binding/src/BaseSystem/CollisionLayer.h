#pragma once
#include <initializer_list>

// 衝突判定レイヤー
enum class ELayer
{
	eNone = -1,
	eTest,
	eFloor,		// 床の衝突判定用
	eWall,		// 壁の衝突判定用
	eDoor,
	eSwitch,
	eGround,
	ePlayer,
	eCat,
	eEnemy,
	eInteractObj,	// 調べるオブジェクト
	eInteractSearch,// 調べるオブジェクト探知用
	eBreakableSearch,	// 壊せるオブジェクト探知用
	eDemon,			// 妖力の攻撃を行えるオブジェクト
	eAttackCol,
};
// 衝突判定レイヤーの初期化リスト
using Layers = std::initializer_list<ELayer>;
