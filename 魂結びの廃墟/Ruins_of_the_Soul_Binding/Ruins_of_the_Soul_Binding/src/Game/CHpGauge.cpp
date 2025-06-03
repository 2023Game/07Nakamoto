#include "CHpGauge.h"

// �o�[�̐F���v�Z
CColor CHpGauge::CalcBarColor() const
{
	// �o�[�̐F��ݒ�
	CColor barColor = CColor::green;
	if (mPercent <= 0.2f) barColor = CColor(1.0f, 0.5f, 0.0f, 1.0f);
	else if (mPercent <= 0.5f) barColor = CColor::yellow;
	return barColor;
}
