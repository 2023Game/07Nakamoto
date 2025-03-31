#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CInteractObject.h"

class CCollider;
class CBillBoard;

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

	// 調べる内容のテキスト画像のパスを返す
	std::string GetInteractTextPath() const override;
	// 調べるUIを表示する座標を返す
	virtual CVector GetInteractUIPos() const override;

	// 描画処理
	void Render() override;

private:
	CModel* mpModel;	// スイッチのモデルデータ
	CCollider* mpCollider;	// スイッチのコリジョンデータ

	bool mSwitch;	// スイッチがオンかオフか
};