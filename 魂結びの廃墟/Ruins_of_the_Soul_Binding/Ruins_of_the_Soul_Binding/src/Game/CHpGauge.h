#pragma once
#include "CGaugeUI.h"

// HP�Q�[�W�N���X
class CHpGauge : public CGaugeUI
{
private:
	// �o�[�̐F���v�Z
	CColor CalcBarColor() const override;
};