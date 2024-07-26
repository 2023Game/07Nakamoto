#ifndef SLOPE_H
#define SLOPE_H

#include "CCharacter.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CSlope : public CCharacter
{
public:

	//デフォルトコンストラクタ
	CSlope();

	/// <summary>
	/// モデルの設定
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡縮</param>
	/// <param name="model">モデル</param>
	void SetSlope(const CVector& pos,
		const CVector& rot, const CVector& scale,CModel *model);

	//更新処理
	void Update();

	//衝突処理
	void Collision();

	CMatrix* GetMatrix();

private:
	//コライダ
	CCollider mColliderSlope;

};

#endif // !SLOPE_H
