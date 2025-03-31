#include "CInteractUI.h"
#include "CCamera.h"
#include <glut.h>
#include "CImage3D.h"
#include "Maths.h"
#include "CInteractObject.h"

#define WORLD_UNIT_PER_PIXEL 32.0f
#define IMAGE_SPACE 0.2f	// ���ׂ�UI�̉摜���m�̊Ԃ̃T�C�Y
#define IMAGE_OFFSET -2.3f	// ���ׂ�UI�̉摜�̃I�t�Z�b�g�l
#define IMAGE_ALPHA 0.8f	// ���ׂ�UI�̉摜�̃A���t�@�l

// �R���X�g���N�^
CInteractUI::CInteractUI()
	: CObjectBase(ETag::eUI, ETaskPriority::eUI3D, 0, ETaskPauseType::eGame)
	, mpButtonImage(nullptr)
	, mpTextImage(nullptr)
{
	// �C���^���N�g�{�^���̃C���[�W��ǂݍ���
	mpButtonImage = new CImage3D
	(
		"UI\\BottonUI\\E.png",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpButtonImage->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	mpButtonImage->SetBillboard(true);
	mpButtonImage->SetDepthTest(false);
	mpButtonImage->SetAlignment(EAlignment::eMiddleRight);
	mpButtonImage->SetAlpha(IMAGE_ALPHA);

	// ���ׂ���e�e�L�X�g�̃C���[�W��ǂݍ���
	mpTextImage = new CImage3D
	(
		"",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpTextImage->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	mpTextImage->SetBillboard(true);
	mpTextImage->SetDepthTest(false);
	mpTextImage->SetAlignment(EAlignment::eMiddleLeft);
	mpTextImage->SetAlpha(IMAGE_ALPHA);
}

// �f�X�g���N�^
CInteractUI::~CInteractUI()
{
	// �ǂݍ��񂾃C���[�W���폜
	SAFE_DELETE(mpButtonImage);
	SAFE_DELETE(mpTextImage);
}

// ���ׂ�UI��\��
void CInteractUI::Show(CInteractObject* obj)
{
	// ���ׂ�I�u�W�F�N�g���Ƃ̓��e�e�L�X�g�̃C���[�W��ǂݍ���
	std::string path = obj->GetInteractTextPath();
	mpTextImage->Load(path);

	// ���ׂ�I�u�W�F�N�g�̍��W�Ɉړ�
	Position(obj->GetInteractUIPos());

	// UI��\��
	SetShow(true);
}

// ���ׂ�UI���\��
void CInteractUI::Hide()
{
	SetShow(false);
}

// �X�V
void CInteractUI::Update()
{
	mpButtonImage->Update();
	mpTextImage->Update();
}

// �`��
void CInteractUI::Render()
{
	// ���S���W���擾
	CVector pos = Position();
	// �J�����̉������x�N�g�����擾
	CCamera* cam = CCamera::CurrentCamera();
	CVector camSide = CVector::Cross(CVector::up, cam->VectorZ());

	// ����{�^���̃C���[�W�̍��W��ݒ肵�āA�`��
	mpButtonImage->Position(pos + camSide * (IMAGE_OFFSET -IMAGE_SPACE * 0.5f));
	mpButtonImage->Render();

	// ���ׂ���e�e�L�X�g�̃C���[�W�̍��W��ݒ肵�āA�`��
	mpTextImage->Position(pos + camSide * (IMAGE_OFFSET + IMAGE_SPACE * 0.5f));
	mpTextImage->Render();
}
