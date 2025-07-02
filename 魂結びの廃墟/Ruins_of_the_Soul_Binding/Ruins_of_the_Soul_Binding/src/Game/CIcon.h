#pragma once
#include "CTexture.h"
#include "CUIBase.h"

class CImage;

class CIcon : public CUIBase
{
public:
	// インスタンスの取得
	static CIcon* Instance();

	// コンストラクタ
	CIcon();
	// デストラクタ
	~CIcon();

	enum class EIcon
	{
		eCat,
		ePlayer,
		eTogether,
	};

	// アイコンの設定
	void SetIcon(int icon);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	static CIcon* spInstance;	// アイコンのインスタンス

	CImage* mpIcon;			// アイコンのイメージ
	CImage* mpIconFrame;	// アイコンのフレーム


};