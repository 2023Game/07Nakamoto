#include "CInteractObject.h"
#include "CInteractObjectManager.h"

#define DEFAULT_TEXT_PATH "UI\\Interact\\interact.png"
#define OFFSET_POS	CVector(0.0f,10.0f,0.0f)

// コンストラクタ
CInteractObject::CInteractObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eInteractObject, prio, sortOrder, pause)
	, mInteractStr("調べる")
	, mInteract(true)
	, mOffSetPos(OFFSET_POS)
#if _DEBUG
	, mDebugName("InteractObj")
#endif
{
	// 管理クラスのリストに自身を追加
	CInteractObjectManager::Instance()->Add(this);
}

// デストラクタ
CInteractObject::~CInteractObject()
{
	// 管理クラスが存在したら、自身をリストから取り除く
	CInteractObjectManager* mgr = CInteractObjectManager::Instance();
	if (mgr != nullptr)
	{
		mgr->Remove(this);
	}
}

// 調べられる状態かどうか
bool CInteractObject::CanInteract() const
{
	return mInteract;
}

// 調べる内容のテキストを返す 
std::string CInteractObject::GetInteractStr() const
{
	return mInteractStr;
}

// 調べる内容のテキスト画像のパスを返す
std::string CInteractObject::GetInteractTextPath() const
{
	return DEFAULT_TEXT_PATH;
}

// 調べるUIを表示する座標を返す
CVector CInteractObject::GetInteractUIPos() const
{
	return Position() + mOffSetPos;
}

// 調べるときに参照するオブジェクトの位置
CVector CInteractObject::GetInteractPos() const
{
	return Position();
}

#if _DEBUG
// デバッグ表示用の名前を取得
std::string CInteractObject::GetDebugName() const
{
	return mDebugName;
}

// デバッグ表示用のm名前を設定
void CInteractObject::SetDebugName(std::string name)
{
	mDebugName = name;
}
#endif