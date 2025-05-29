#include "CInteractObject.h"

#define DEFAULT_TEXT_PATH "UI\\Interact\\interact.png"

// コンストラクタ
CInteractObject::CInteractObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eInteractObject, prio, sortOrder, pause)
	, mInteractStr("調べる")
	, mHp(100)
#if _DEBUG
	, mDebugName("InteractObj")
#endif
{
}

// デストラクタ
CInteractObject::~CInteractObject()
{
}

// 調べられる状態かどうか
bool CInteractObject::CanInteract() const
{
	return true;
}

void CInteractObject::TakeDamage(int damege, CObjectBase* causer)
{
	if (damege >= mHp)
	{
		mHp = 0;
		Kill();
	}
	else
	{
		mHp -= damege;
	}
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
	return Position() + CVector(0.0f, 10.0f, 0.0f);
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