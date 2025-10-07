#pragma once
#include "CObjectBase.h"
#include "CCollider.h"
#include "CModel.h"

class CDoor : public CObjectBase
{
public:
	// コンストラクタ
	CDoor(CModel* model, const CVector& pos);
	// デストラクタ
	~CDoor();

	// 更新
	void Update();
	// 描画
	void Render();

private:
	CModel* mpModel;

};