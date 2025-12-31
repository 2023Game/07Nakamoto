#pragma once
#include "CObjectBase.h"
#include "CCollider.h"
#include "CModel.h"

class CDoor : public CObjectBase
{
public:
	// コンストラクタ
	CDoor(const CVector& pos);
	// デストラクタ
	~CDoor();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CModel* mpModel;
	
};