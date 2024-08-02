#ifndef SLOPE_H
#define SLOPE_H

#include "CCharacter.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CSlope : public CCharacter
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡縮</param>
	/// <param name="model">モデル</param>
	CSlope(const CVector& pos,
		const CVector& rot, const CVector& scale,CModel *model);

	//衝突処理
	//void Collision();

	CMatrix* GetMatrix();

	void Update();

	//坂のモデルを取得する
	static CModel* GetModelSlope();

private:
	static CModel mModelSlope;

	//コライダ
	CColliderMesh mCSlope;

};

#endif // !SLOPE_H
