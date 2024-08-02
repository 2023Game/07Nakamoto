#ifndef CFLOOR_H
#define CFLOOR_H

#include "CCollider.h"
#include "CColliderMesh.h"
#include "CModel.h"

class CFloor:public CCharacter
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡縮</param>
	/// <param name="model">モデル</param>
	CFloor(const CVector& pos,
		const CVector& rot, const CVector& scale, CModel* model);

	//坂のモデルを取得する
	static CModel* GetModelFloor();
private:
	static CModel mModelFloor;
	//コライダ
	CColliderMesh mCFloor;
};

#endif // !CFLOOR_H

