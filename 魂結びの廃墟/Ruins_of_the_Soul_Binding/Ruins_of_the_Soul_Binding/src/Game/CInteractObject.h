#pragma once
#include "CObjectBase.h"

class CInteractObject : public CObjectBase
{
public:
	// コンストラクタ
	CInteractObject(
		ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// デストラクタ
	virtual ~CInteractObject();

	// 調べられる状態かどうか
	virtual bool CanInteract() const;
	// 調べる（継承先で実装）
	virtual void Interact() = 0;

	// 調べる内容のテキストを返す 
	std::string GetInteractStr() const;
	// 調べる内容のテキスト画像のパスを返す
	virtual std::string GetInteractTextPath() const;
	// 調べるキー画像のパスを返す
	virtual std::string GetInteractKeyPath() const;
	// 調べるUIを表示する座標を返す
	virtual CVector GetInteractUIPos() const;

	// 調べるときに参照するオブジェクトの位置
	virtual CVector GetInteractPos() const;

#if _DEBUG
	// デバッグ表示用の名前を取得
	std::string GetDebugName() const;
	// デバッグ表示用のm名前を設定
	void SetDebugName(std::string name);
#endif

protected:
	CVector mOffSetPos;	// 座標のオフセット

	std::string mInteractStr;	// 調べる内容のテキスト
	bool mInteract;		// 調べられる状態か

#if _DEBUG
	std::string mDebugName;		// デバッグ表示用の名前
#endif
};