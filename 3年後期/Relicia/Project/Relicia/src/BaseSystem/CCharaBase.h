#pragma once
#include "CObjectBase.h"
#include "CrystalData.h"

/// <summary>
/// キャラクターのベースクラス
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	// コンストラクタ
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	// デストラクタ
	virtual ~CCharaBase();

	// 最大HPを取得
	int GetMaxHp() const;
	// 現在HPを取得
	int GetHp() const;

	// ダメージを受ける
	virtual void TakeDamage(int damage, CObjectBase* causer);
	// 属性ダメージを受ける
	virtual void TakeDamage(int damage, ElementType type, CObjectBase* causer);

	// 死亡
	virtual void Death();

protected:
};
