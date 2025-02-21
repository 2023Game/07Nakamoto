#pragma once
#include "CUIBase.h"
#include "CTask.h"

class CText;
class CImage;
class CButton;

// アイテム選択中のメニュー
class CItemMenu : public CUIBase
{
public:
	// インスタンスを取得
	static CItemMenu* Instance();

	// コンストラクト
	CItemMenu();
	// デストラクタ
	~CItemMenu();

	// 表示する
	void Open();
	// 閉じる
	void Close();

	// アイテムを使うか
	bool IsUse() const;
	// アイテムメニューを閉じるか
	bool IsClose() const;

	// 更新
	void Update();
	// 描画
	void Render();

private:
	static CItemMenu* spInstance;

	// 待機状態
	void UpdateIdle();
	// メニュー選択
	void UpdateSelect();

	// 内容の種類
	enum class EState
	{
		eIdle,		// 待機状態
		eOpen,		// メニューを開く
		eSelect,	// メニュー選択
		eUse,		// アイテムを使う
		eBack,		// 戻る
	};

	// 状態切り替え
	void ChangeState(EState state);

	// [Use]クリック時のコールバック関数
	void OnClickUse();
	// [Back]クリック時のコールバック関数
	void OnClickClose();

	EState mState;	// 現在の状態
	int mStateStep;	// 状態内でのステップ管理用

	CImage* mpBack;	// 文字の描画の背景
	CText* mpText;	// 文字列描画用

	bool mIsOpened; // アイテムメニューが開いているか
	bool mDecision;	// 決定フラグ
	int mSelectIndex; // /現在選択されている項目

	std::vector<CButton*> mButtons;	// ボタン格納用

};