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

// デストラクタ
CGameSceneBase::~CGameSceneBase()
{
	// 次に読み込まれるシーンがゲームシーンでなければ、
	// アイテムのリソースを全て破棄する
	CSceneManager* sceneMgr = CSceneManager::Instance();
	if (sceneMgr != nullptr && !sceneMgr->IsNextGameScene())
	{
		Item::DeleteItemResources();
	}

}

//シーン読み込み
void CGameSceneBase::Load()
{
	// アイテムのリソースを全読み込む
	Item::LoadItemResources();
	// 経路探索管理クラスを作成
	new CNavManager();
	// フィールド作成
	new CField();
	// ゲームメニューを作成
	new CInventory();
}

// 更新処理
void CGameSceneBase::Update()
{
	// BGM再生中でなければ、BGMを再生
	//if (!mpGameBGM->IsPlaying())
	//{
	//	mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
	//}

	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eGameOver);
	}

	// ゲームメニューを開いてなければ、[Ｍ]キーでメニューを開く
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
