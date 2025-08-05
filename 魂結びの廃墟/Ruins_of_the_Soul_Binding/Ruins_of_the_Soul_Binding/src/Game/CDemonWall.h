#pragma once
#include "CObjectBase.h"

class CModel;
class CCollider;
class CDemonPowar;

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
	CDemonPowar* mpDemonPowar;	// 連携させる妖力の源
};