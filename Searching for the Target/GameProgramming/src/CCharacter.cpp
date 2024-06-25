#include "CCharacter.h"
#include "CApplication.h"

//デフォルトコンストラクタ
CCharacter::CCharacter()
	:mpModel(nullptr)
	,mTag(ETag::ENULL)
{
	//タスクリストに追加
	CTaskManager::GetInstance()->Add(this);
}
//デストラクタ
CCharacter::~CCharacter()
{
	//タスクリストから削除
	CTaskManager::GetInstance()->Remove(this);
}

//コンストラクタ
CCharacter::CCharacter(int priority)
	:mpModel(nullptr)
	, mTag(ETag::ENULL)
{
	mPriority = priority;
	CTaskManager::GetInstance()->Add(this);
}

//モデルの設定
void CCharacter::SetModel(CModel* m)
{
	mpModel = m;
}

//タグの設定
CCharacter::ETag CCharacter::SetTag(ETag tag)
{
	return mTag = tag;
}

//タグの取得
CCharacter::ETag CCharacter::GetTag()
{
	return mTag;
}

//描画処理
void CCharacter::Render()
{
	mpModel->Render(mMatrix);
}