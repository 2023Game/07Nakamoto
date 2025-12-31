#pragma once
#include "CObjectBase.h"

class CModel;
class CCollider;

//脱出エリアクラス
class CEscapeArea :public CObjectBase
{
public:
	// コンストラクタ
	CEscapeArea(const CVector& pos, const CVector& angle, const CVector& size);
	// デストラクタ
	~CEscapeArea();

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画処理
	void Render() override;

private:
	CModel* mpModel;	// 脱出エリアのモデルデータ
	CCollider* mpCollider;	// 脱出エリアのコリジョンデータ

};