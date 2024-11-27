#pragma once
#include "CObjectBase.h"
#include "CModel.h"

class CTorch : public CObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="angle">回転</param>
	/// <param name="size">大きさ</param>
	CTorch(const CVector& pos, const CVector& angle, const CVector& size);

	// デストラクタ
	~CTorch();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	CModel* mpModel;	// モデルデータ


};