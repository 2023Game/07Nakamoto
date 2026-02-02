#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;

class CResultUI : public CTask
{
public:
	// コンストラクタ
	CResultUI();
	// デストラクタ
	~CResultUI();

	// リザルト画面を終了するか
	bool IsEnd() const;
	// ゲームをコンティニューするか
	bool IsContinue() const;
	// タイトルへ戻るか
	bool IsTitle();
	// ゲームを終了するか
	//bool IsExitGame() const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// 待機状態
	void UpdateIdle();
	// フェードアウト
	void UpdateFadeOut();

	// シーンの状態
	enum class EState
	{
		eIdle,		// 待機状態
		eFadeOut,	// フェードアウト
	};

	// 状態切り替え
	void ChangeState(EState state);

	// [コンティニュー]クリック時のコールバック関数
	void OnClickStart();
	// [タイトルへ]クリック時のコールバック関数
	void OnClickTitle();
	// [おわる]クリック時のコールバック関数
	//void OnClickQuit();

	EState mState;		// 現在の状態
	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用
	bool mIsEnd;		// リザルト画面の終了フラグ
	int mSelectIndex;	// 現在選択している項目

	CImage* mpResultBg;	// リザルト画面の背景イメージ
	std::vector<CExpandButton*> mButtons;

	int mTotalSellPrice;	// インベントリの合計売却価格
	CFont* mpLogoFont;	// ロゴのフォント
	CText* mpTotalSellPriceText;	// 合計価格のテキスト

};