#include "CHpUI.h"
#include "CImage.h"
#include "CPlayer2.h"

CHpUI::CHpUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
{
	// �_���[�W�Q�[�W�𐶐�
	mpRedBar = new CImage
	(
		"UI\\hp_red.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	// HP�Q�[�W�𐶐�
	mpBlueBae = new CImage
	(
		"UI\\hp_blue.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
}

CHpUI::~CHpUI()
{
	SAFE_DELETE(mpRedBar);
	SAFE_DELETE(mpBlueBae);
}

void CHpUI::SetPos(const CVector2& pos)
{
}

void CHpUI::Update()
{
	CPlayer2* player = CPlayer2::Instance();
	int hp = player->GetHp();
}

void CHpUI::Render()
{
	mpRedBar->Render();
	mpBlueBae->Render();
}
