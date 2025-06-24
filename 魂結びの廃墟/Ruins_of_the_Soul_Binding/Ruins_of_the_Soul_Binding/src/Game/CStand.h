#pragma once
#include "CObjectBase.h"

class CModel;
class CCollider;

// スイッチの土台クラス
class CStand : public CObjectBase
{
public:
	//コンストラクタ
	CStand(const CVector& pos);
	// デストラクタ
	~CStand();

	// 描画
	void Render();

private:
	CModel* mpStandModel;
	CCollider* mpStandWallColMesh;	// 土台の壁部分のコライダー
	CCollider* mpStandFloorColMesh;	// 土台の床部分のコライダー
};