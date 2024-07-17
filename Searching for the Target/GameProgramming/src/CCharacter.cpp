#include "CCharacter.h"
#include "CApplication.h"

//デフォルトコンストラクタ
CCharacter::CCharacter()
	:mpModel(nullptr)
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
	: mpModel(nullptr)
{
	mPriority = priority;
	CTaskManager::GetInstance()->Add(this);
}

//有効フラグの取得
bool CCharacter::GetEnabled()
{
	return mEnabled;
}

//モデルの設定
void CCharacter::SetModel(CModel* m)
{
	mpModel = m;
}

//描画処理
void CCharacter::Render()
{
	mpModel->Render(mMatrix);
}