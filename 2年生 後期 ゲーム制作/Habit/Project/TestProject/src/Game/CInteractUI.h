#pragma once
#include "CObjectBase.h"

class CImage3D;

// 3D空間に表示するインタラクトボタンのUI
class CInteractUI :public CObjectBase
{
public:
	// コンストラクタ
	CInteractUI(CObjectBase* owner);
	// デストラクタ
	~CInteractUI();

	// 持ち主を設定
	void SetOwner(CObjectBase* owner);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CObjectBase* mpOwner;	// インタラクトボタンの持ち主
	CImage3D* mpInteractUI;	// インタラクトボタンのイメージ
	CVector2 mGaugeSize;	// ゲージのイメージのサイズ

};