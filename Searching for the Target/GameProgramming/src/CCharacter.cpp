#include "CCharacter.h"
#include "CApplication.h"

//コンストラクタ
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