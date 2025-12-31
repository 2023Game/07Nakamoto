#pragma once
#include "CObjectBase.h"
#include "CCollider.h"
#include "CModel.h"

class CWall : public CObjectBase
{
public:
	// コンストラクタ
	CWall(const CVector& pos);
	// デストラクタ
	~CWall();

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
	CModel* mpModel;
};