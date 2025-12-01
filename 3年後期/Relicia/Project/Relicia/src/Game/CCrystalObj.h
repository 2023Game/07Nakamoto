#pragma once
#include "CObjectBase.h"
#include "CrystalData.h"

class CCollider;
class CModel;

// クリスタルの
class CCrystalObj : public CObjectBase
{
public:
	// コンストラクタ
	CCrystalObj(ElementType type, CVector pos);
	// デストラクタ
	~CCrystalObj();

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;
private:
	ElementType mElementType;	// 属性の種類
	const CrystalData* mpCrystalData;	// クリスタルのデータのポインタ
	CModel* mpModel;			// クリスタルのモデルデータ

	CCollider* mpCollider;		// クリスタルのコライダー
};