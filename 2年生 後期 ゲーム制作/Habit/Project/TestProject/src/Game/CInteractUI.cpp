#include "CInteractUI.h"
#include "CCamera.h"
#include <glut.h>
#include "CImage3D.h"
#include "Maths.h"

#define WORLD_UNIT_PER_PIXEL 32.0f

// �R���X�g���N�^
CInteractUI::CInteractUI(CObjectBase* owner)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI3D, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mpInteractUI(nullptr)
{
	// �C���^���N�g�{�^���̃C���[�W��ǂݍ���
	mpInteractUI = new CImage3D
	(
		"UI\\BottonUI\\E.png",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpInteractUI->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	mpInteractUI->SetDepthMask(true);
	mGaugeSize = mpInteractUI->GetSize();

}

// �f�X�g���N�^
CInteractUI::~CInteractUI()
{
	// �ǂݍ��񂾃C���[�W���폜
	SAFE_DELETE(mpInteractUI);

	// ������ɍ폜���ꂽ���Ƃ�`����
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// �������ݒ�
void CInteractUI::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// �X�V
void CInteractUI::Update()
{
	mpInteractUI->Update();
}

// �`��
void CInteractUI::Render()
{
	// �s��̕ۑ�
	glPushMatrix();

	// ��ɃJ�����̕�������������
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	// �C���^���N�g�{�^���̕`��
	mpInteractUI->Render();

	// �s���߂�
	glPopMatrix();
}
