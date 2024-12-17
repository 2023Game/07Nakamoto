#include "CHpUI.h"
#include "CImage.h"
#include "CPlayer2.h"
#include "Maths.h"

CHpUI::CHpUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpBlueBar(nullptr)
	, mpRedBar(nullptr)
	, mBaseBarSize(0.0f, 0.0f)
	, mMaxPoint(100)
	, mCurPoint(mMaxPoint)
	, mPercent(1.0f)
{
	SetPos(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5);

	// �_���[�W�Q�[�W�𐶐�
	mpRedBar = new CImage
	(
		"UI\\hp_red.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);

	// HP�Q�[�W�𐶐�
	mpBlueBar = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mBaseBarSize = mpRedBar->GetSize();
	CVector2 center = CVector2(-mBaseBarSize.X() * 0.5f, 0.0f);
	mpBlueBar->SetCenter(center);
}

CHpUI::~CHpUI()
{
	SAFE_DELETE(mpRedBar);
	SAFE_DELETE(mpBlueBar);
}

// �ő�l��ݒ�
void CHpUI::SetMaxPoint(int point)
{
	mMaxPoint = point;
	ApplyPoint();
}

// ���ݒl��ݒ�
void CHpUI::SetCurPoint(int point)
{
	mCurPoint = point;
	ApplyPoint();
}

// ���݂̃|�C���g���Q�[�W�ɔ��f
void CHpUI::ApplyPoint()
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

// �X�V
void CHpUI::Update()
{

}

// �`��
void CHpUI::Render()
{
	CVector2 pos = mPosition;

	// �Q�[�W�̕`��
	mpRedBar->SetPos(pos);
	mpRedBar->Render();
	
	// �o�[�̍��W��ݒ�
	CVector2 barPos = mPosition;
	barPos.X(barPos.X() - mBaseBarSize.X() * 0.5f);
	mpBlueBar->SetPos(barPos);
	// �|�C���g�c�ʂ̊����ɍ��킹�āA�o�[�T�C�Y��ύX
	CVector2 barSize = mBaseBarSize;
	barSize.X(barSize.X() * mPercent);
	mpBlueBar->SetSize(barSize);
	// �o�[�̐F��ݒ�
	CColor barColor = CColor::green;
	if (mPercent <= 0.2f) barColor = CColor::red;
	else if (mPercent <= 0.5f) barColor = CColor::yellow;
	mpBlueBar->SetColor(barColor);
	// �o�[�̕`��
	mpBlueBar->Render();
}

