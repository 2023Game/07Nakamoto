#pragma once
#include "CInteractObject.h"
#include "ItemData.h"
#include "CModel.h"

class CCollider;

// 3D空間に配置するアイテムのベースクラス
class CItemObj :public CInteractObject
{
public:
	// コンストラクタ
	CItemObj(ItemType type);
	// デストラクタ
	virtual ~CItemObj() = 0;

	// 調べられる状態かどうか
	bool CanInteract() const override;
	// 調べる
	void Interact() override;

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

protected:
	// コライダー作成(継承先で上書き可)
	virtual void CreateCollider();

	ItemType mItemTyope;	// アイテムの種類
	const ItemData* mpItemData;	// アイテムデータのポインタ
	CModel* mpModel;		// アイテムのモデルデータ
	CCollider* mpCollider;	// アイテムのコライダー

};