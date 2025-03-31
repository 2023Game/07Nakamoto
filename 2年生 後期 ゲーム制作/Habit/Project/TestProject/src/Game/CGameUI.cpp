#include "CGameUI.h"
#include "CInteractUI.h"
#include "CInteractObject.h"

// CGameUI�̃C���X�^���X�̕ϐ���`
CGameUI* CGameUI::spInstance = nullptr;

// �C���X�^���X���擾����
CGameUI* CGameUI::Instance()
{
	return spInstance;
}

// �C���X�^���X��j������
void CGameUI::ClearInstance()
{
	SAFE_DELETE(spInstance);
}

// �R���X�g���N�^
CGameUI::CGameUI()
	:mpInteractUI(nullptr)
{
	if (spInstance != nullptr)
	{
		SAFE_DELETE(spInstance);
	}
	spInstance = this;

	mpInteractUI = new CInteractUI();
}

// �f�X�g���N�^
CGameUI::~CGameUI()
{
}

// �w�肵���I�u�W�F�N�g�̒��ׂ�UI��\��
void CGameUI::ShowInteractUI(CInteractObject* obj)
{
	mpInteractUI->Show(obj);
}

// ���ׂ�UI���\���ɂ���
void CGameUI::HideInteractUI()
{
	mpInteractUI->Hide();
}
