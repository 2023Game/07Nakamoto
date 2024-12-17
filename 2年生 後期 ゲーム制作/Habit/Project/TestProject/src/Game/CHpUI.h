#pragma once
#include "CTask.h"
#include "CTexture.h"
#include "CUIBase.h"

class CImage;

class CHpUI : public CTask
{
public:
	// コントラクタ
	CHpUI();
	// デストラクタ
	~CHpUI();

	//表示座標を設定（CVector2版）
	void SetPos(const CVector2& pos);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CImage* mpBlueBae;
	CImage* mpRedBar;

};