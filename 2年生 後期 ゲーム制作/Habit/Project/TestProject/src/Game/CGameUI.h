#pragma once
#include "CTask.h"

class CImage;

class CGameUI : public CTask
{
public:
	// コントラクタ
	CGameUI();
	// デストラクタ
	~CGameUI();

	// 更新
	void Update() override;
	// 描画
	void Render() override;
};