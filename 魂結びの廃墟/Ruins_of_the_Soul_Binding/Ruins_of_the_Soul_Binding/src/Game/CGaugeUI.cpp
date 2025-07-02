#include "CGaugeUI.h"
#include "CImage.h"
#include "Maths.h"
#include "CIcon.h"

CGaugeUI::CGaugeUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpWhiteImag(nullptr)
	, mpGaugeImag(nullptr)
	, mBaseBarSize(0.0f, 0.0f)
	, mMaxPoint(100)
	, mCurPoint(mMaxPoint)
	, mPercent(1.0f)
{
	// �����̍��W
	//SetPos(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5);

	// �_���[�W�Q�[�W�𐶐�
	mpGaugeImag = new CImage
	(
		"UI\\hp_redbar.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mBaseBarSize = mpGaugeImag->GetSize();

	// HP�Q�[�W�𐶐�
	mpWhiteImag = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	CVector2 center = CVector2(-mBaseBarSize.X() * 0.5f, 0.0f);
	mpWhiteImag->SetCenter(center);
}

CGaugeUI::~CGaugeUI()
{
	SAFE_DELETE(mpGaugeImag);
	SAFE_DELETE(mpWhiteImag);

}

// �ő�l��ݒ�
void CGaugeUI::SetMaxPoint(int point)
{
	mMaxPoint = point;
	ApplyPoint();
}

// ���ݒl��ݒ�
void CGaugeUI::SetCurPoint(int point)
{
	mCurPoint = point;
	ApplyPoint();
}

// ���݂̃|�C���g���Q�[�W�ɔ��f
void CGaugeUI::ApplyPoint()
{
	// �ő�l���s���l�łȂ���΁A
	if (mMaxPoint > 0)
	{
		// �c��̒l���犄�������߂�
		mPercent = Math::Clamp01((float)mCurPoint / mMaxPoint);
	}
	// �s���l�������ꍇ�́A1.0�ŌŒ�
	else
	{
		mPercent = 1.0f;
	}
}

CColor CGaugeUI::CalcBarColor() const
{
	return CColor::white;
}

// �X�V
void CGaugeUI::Update()
{
}

// �`��
void CGaugeUI::Render()
{
	CVector2 pos = mPosition;

	// �Q�[�W�̕`��
	mpGaugeImag->SetPos(pos);
	mpGaugeImag->Render();
	
	// �o�[�̍��W��ݒ�
	CVector2 barPos = mPosition;
	barPos.X(barPos.X() - mBaseBarSize.X() * 0.5f);
	mpWhiteImag->SetPos(barPos);
	// �|�C���g�c�ʂ̊����ɍ��킹�āA�o�[�T�C�Y��ύX
	CVector2 barSize = mBaseBarSize;
	barSize.X(barSize.X() * mPercent);
	mpWhiteImag->SetSize(barSize);
	// �o�[�̐F��ݒ�
	CColor barColor = CalcBarColor();
	mpWhiteImag->SetColor(barColor);
	// �o�[�̕`��
	mpWhiteImag->Render();
}

