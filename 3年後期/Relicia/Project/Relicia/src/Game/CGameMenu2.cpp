#include "CGameMenu2.h"

// コンストラクタ
CGameManu2::CGameManu2()
	: CUIBase(ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,false, false)
	, mState(EState::eIdle)
	, mpBack(nullptr)
	, mpText(nullptr)
	, mIsOpened(false)
	, mDecision(false)
	, mSelectIndex(-1)
{
}

// デストラクタ
CGameManu2::~CGameManu2()
{
}

// 表示する
void CGameManu2::Open()
{
}

// 閉じる
void CGameManu2::Close()
{
}

// アイテムを使う
bool CGameManu2::IsUse() const
{
	return false;
}

// アイテムを装備するか
bool CGameManu2::IsEquipment() const
{
	return false;
}

// アイテムメニューが閉じているか
bool CGameManu2::IsClose() const
{
	return false;
}

// 更新処理
void CGameManu2::Update()
{
}

// 描画処理
void CGameManu2::Render()
{
}

// 待機状態
void CGameManu2::UpdateIdle()
{
}

// メニュー選択
void CGameManu2::UpdateSelect()
{
}

// 状態切り替え
void CGameManu2::ChangeState(EState state)
{
}

// [使う]クリック時のコールバック関数
void CGameManu2::OnClickUse()
{
}

// [装備]クリック時のコールバック関数
void CGameManu2::OnClickEquipment()
{
}

// [戻る]クリック時のコールバック関数
void CGameManu2::OnClickClose()
{
}
