#pragma once
#include "CObjectBase.h"
#include "CCollider.h"
#include "CModel.h"

class CFloor : public CObjectBase
{
public:
	// コンストラクタ
	CFloor(const CVector& pos);
	// デストラクタ
	~CFloor();

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
	CModel* mpModel;
};