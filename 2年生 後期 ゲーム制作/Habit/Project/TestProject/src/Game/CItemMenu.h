#pragma once
#include "CUIBase.h"
#include "CTask.h"

class CText;
class CImage;

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

	// 開く
	void Open();
	// 閉じる
	void Close();
	// インベントリが開いているかどうか
	bool IsOpened() const;

	// 更新
	void Update();
	// 描画
	void Render();

private:
	static CItemMenu* spInstance;

	CImage* mpBack;	// 文字の描画の背景
	CText* mpText;	// 文字列描画用

	// 内容の種類
	enum EContent
	{
		eUse,	// 使う
		eBack,	// 戻る
	};

	// アイテムメニュー
	struct MenuInfo
	{
		std::string text;	// テキスト
		EContent content;	// 内容
	};
	// アイテムメニューのリスト
	std::vector<MenuInfo> mMenuList;

	bool mIsOpened;
};