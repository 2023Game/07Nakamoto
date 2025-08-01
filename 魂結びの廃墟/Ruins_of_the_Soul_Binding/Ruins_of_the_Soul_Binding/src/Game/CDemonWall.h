#pragma once
#include "CObjectBase.h"

class CModel;
class CCollider;

class CDemonWall : public CObjectBase
{
public:
	// コンストラクタ
	CDemonWall();
	// デストラクタ
	~CDemonWall();

	// 描画
	void Render() override;

private:
	CModel* mpModel;	// モデルデータ
	CCollider* mpColliderMesh;	// コライダー
};