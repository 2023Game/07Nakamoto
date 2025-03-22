#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CInteractObject.h"

class CCollider;
class CBillBoard;
class CInteractUI;

// スイッチのクラス
class CPushSwitch : public CInteractObject
{
public:
	// コンストラクタ
	CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size);
	// デストラクタ
	~CPushSwitch();
	
	// スイッチが押されたかどうか
	bool IsOnSwtch();

	// 調べる
	void Interact() override;

	// 描画処理
	void Render() override;

private:
	CModel* mpModel;	// スイッチのモデルデータ
	CCollider* mpCollider;	// スイッチのコリジョンデータ
	CInteractUI* mpInteractUI;	// インタラクトボタンのイメージ

	bool mSwitch;	// スイッチがオンかオフか
};