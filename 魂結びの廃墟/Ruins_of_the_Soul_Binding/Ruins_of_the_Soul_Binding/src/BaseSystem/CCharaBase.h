#pragma once
#include "CObjectBase.h"

/// <summary>
/// �L�����N�^�[�̃x�[�X�N���X
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	// �R���X�g���N�^
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	// �f�X�g���N�^
	virtual ~CCharaBase();
};
