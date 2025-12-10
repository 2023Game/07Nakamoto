#pragma once
#include "CInteractObject.h"
#include "ItemData.h"

class CCollider;
class CImage3D;

// 3D空間に配置するアイテムのベースクラス
class CItemObj :public CInteractObject
{
public:
	// コンストラクタ
	CItemObj(ItemType type, CVector pos);
	// デストラクタ
	~CItemObj();

	// 調べられる状態かどうか
	bool CanInteract() const override;
	// 調べる
	void Interact() override;

	// 調べる内容のテキスト画像のパスを返す
	std::string GetInteractTextPath() const override;
	// 調べるUIを表示する座標を返す
	virtual CVector GetInteractUIPos() const override;

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

protected:
	// コライダー作成(継承先で上書き可)
	virtual void CreateCollider();

	ItemType mItemType;	// アイテムの種類
	const ItemData* mpItemData;	// アイテムデータのポインタ
	CImage3D* mpItemImage;		// アイテムのイメージデータ
	CCollider* mpCollider;	// アイテムのコライダー
};