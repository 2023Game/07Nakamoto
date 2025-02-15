#pragma once
#include "CUIBase.h"
#include "CTask.h"

class CText;
class CImage;
class CExpandButton;

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

	// 更新
	void Update();
	// 描画
	void Render();

private:
	static CItemMenu* spInstance;

	// 待機状態
	void UpdateIdle();
	// メニューを開く
	void UpdateOpen();
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
	void OnClickBack();

	EState mState;	// 現在の状態
	int mStateStep;	// 状態内でのステップ管理用

	CImage* mpBack;	// 文字の描画の背景
	CText* mpText;	// 文字列描画用

	bool mIsOpened; // アイテムメニューが開いているか

	std::vector<CExpandButton*> mButtons;	// ボタン格納用

};