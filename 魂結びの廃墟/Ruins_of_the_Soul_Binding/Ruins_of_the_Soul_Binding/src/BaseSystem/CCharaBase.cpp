#include "CCharaBase.h"

// コンストラクタ
CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
{
}

// デストラクタ
CCharaBase::~CCharaBase()
{
}