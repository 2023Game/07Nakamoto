#pragma once
#include "CObjectBase.h"
#include "CCollider.h"
#include "CModel.h"

class CPillar : public CObjectBase
{
public:
	// コンストラクタ
	CPillar(const CVector& pos);
	// デストラクタ
	~CPillar();

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
	CModel* mpModel;
};