#include "CIcon.h"
#include "CImage.h"

#define ICON_CAT_PATH "UI\\Silhouette\\cat.png"
#define ICON_GIRL_PATH "UI\\Silhouette\\girl.png"
#define ICON_CAT_GIRL_PATH "UI\\Silhouette\\cat_girl.png"

#define ICON_FRAME_PATH "UI\\Silhouette\\frame.png"

// �A�C�R���̃C���X�^���X�̕ϐ���`
CIcon* CIcon::spInstance = nullptr;

// �C���X�^���X�̎擾
CIcon* CIcon::Instance()
{
	return spInstance;
}

CIcon::CIcon()
	: mpIcon(nullptr)
	, mpIconFrame(nullptr)
{
	spInstance = this;

	// �A�C�R���𐶐�
	mpIcon = new CImage
	(
		ICON_CAT_GIRL_PATH,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mpIcon->SetPos(CVector2(0.0f,40.0f));

	// �A�C�R���̃t���[��
	mpIconFrame = new CImage
	(
		ICON_FRAME_PATH,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mpIconFrame->SetPos(CVector2(0.0f, 40.0f));
}

// �f�X�g���N�^
CIcon::~CIcon()
{
	SAFE_DELETE(mpIcon);
	SAFE_DELETE(mpIconFrame);
}

// �A�C�R���̐ݒ�
void CIcon::SetIcon(int icon)
{
	if (icon == (int)EIcon::eCat)
	{
		mpIcon->Load(ICON_CAT_PATH);
	}
	else if (icon == (int)EIcon::ePlayer)
	{
		mpIcon->Load(ICON_GIRL_PATH);
	}
	else if (icon == (int)EIcon::eTogether)
	{
		mpIcon->Load(ICON_CAT_GIRL_PATH);
	}

}

// �X�V
void CIcon::Update()
{
}

// �`��
void CIcon::Render()
{
	// �A�C�R���̕`��
	mpIcon->Render();
	// �A�C�R���̃t���[���`��
	mpIconFrame->Render();
}

