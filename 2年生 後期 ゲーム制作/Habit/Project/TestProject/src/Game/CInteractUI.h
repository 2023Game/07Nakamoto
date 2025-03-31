#pragma once
#include "CObjectBase.h"

class CImage3D;
class CInteractObject;

// 3D空間に表示するインタラクトボタンのUI
class CInteractUI :public CObjectBase
{
public:
	// コンストラクタ
	CInteractUI();
	// デストラクタ
	~CInteractUI();

	// 調べるUIを表示
	void Show(CInteractObject* obj);
	// 調べるUIを非表示
	void Hide();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CImage3D* mpButtonImage;	// 操作ボタンのイメージ
	CImage3D* mpTextImage;		// 調べる内容テキストのイメージ

};