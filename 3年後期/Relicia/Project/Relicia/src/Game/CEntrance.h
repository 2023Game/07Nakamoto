#pragma once
#include "CObjectBase.h"
#include "CCollider.h"
#include "CModel.h"

class CEntrance : public CObjectBase
{
public:
	// コンストラクタ
	CEntrance(const CVector& pos);
	// デストラクタ
	~CEntrance();

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
	CModel* mpModel;
};