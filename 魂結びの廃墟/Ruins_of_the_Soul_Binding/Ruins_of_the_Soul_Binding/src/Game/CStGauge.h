#pragma once
#include "CGaugeUI.h"

// �X�^�~�i�Q�[�W�N���X
class CStGauge : public CGaugeUI
{
private:
	// �o�[�̐F���v�Z
	CColor CalcBarColor() const override;
};