#pragma once
#include "CObjectBase.h"

class CModel;

class CSwitchFloor : public CObjectBase
{
public:
	// コンストラクタ
	CSwitchFloor(const CVector& pos);
	// デストラクタ
	~CSwitchFloor();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;
private:
	CModel* mpModel;
};