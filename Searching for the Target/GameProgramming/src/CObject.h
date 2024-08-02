#ifndef COBJECT_H
#define COBJECT_H

#include "CCollider.h"
#include "CColliderMesh.h"
#include "CModel.h"

class CObject :public CCharacter
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡縮</param>
	/// <param name="model">モデル</param>
	CObject(const CVector& pos,
		const CVector& rot, const CVector& scale, CModel* model);

	//坂のモデルを取得する
	static CModel* GetModelObject();
private:
	static CModel mModelObject;
	//コライダ
	CColliderMesh mCObject;
};

#endif // !COBJECT_H

