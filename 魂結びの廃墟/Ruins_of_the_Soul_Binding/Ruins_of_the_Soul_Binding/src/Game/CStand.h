#pragma once
#include "CObjectBase.h"

class CModel;
class CCollider;

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
	CCollider* mpStandColMesh;

};