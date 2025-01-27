#include "CGameSceneBase.h"
#include "CInput.h"
#include "CBGMManager.h"
#include "CNavManager.h"
#include "CField.h"
#include "ItemData.h"
#include "CSceneManager.h"
#include "CInventory.h"


CGameSceneBase::CGameSceneBase(EScene scene)
	: CSceneBase(scene)
	, mpInventory(nullptr)
{
}

// �f�X�g���N�^
CGameSceneBase::~CGameSceneBase()
{
	// ���ɓǂݍ��܂��V�[�����Q�[���V�[���łȂ���΁A
	// �A�C�e���̃��\�[�X��S�Ĕj������
	CSceneManager* sceneMgr = CSceneManager::Instance();
	if (sceneMgr != nullptr && !sceneMgr->IsNextGameScene())
	{
		Item::DeleteItemResources();
	}

}

//�V�[���ǂݍ���
void CGameSceneBase::Load()
{
	// �A�C�e���̃��\�[�X��S�ǂݍ���
	Item::LoadItemResources();
	// �o�H�T���Ǘ��N���X���쐬
	new CNavManager();
	// �t�B�[���h�쐬
	new CField();
	// �Q�[�����j���[���쐬
	new CInventory();
}

// �X�V����
void CGameSceneBase::Update()
{
	// BGM�Đ����łȂ���΁ABGM���Đ�
	//if (!mpGameBGM->IsPlaying())
	//{
	//	mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
	//}

	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eGameOver);
	}

	// �Q�[�����j���[���J���ĂȂ���΁A[�l]�L�[�Ń��j���[���J��
	CInventory* inv = CInventory::Instance();
	if (CInput::PushKey(VK_TAB))
	{
		if (!inv->IsOpened())
		{
			inv->Open();
		}
		else
		{
			inv->Close();
		}
	}
}
