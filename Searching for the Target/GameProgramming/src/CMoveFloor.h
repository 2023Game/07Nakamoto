#ifndef CMOVEFLOOR_H
#define CMOVEFLOOR_H

#include "CCharacter.h"
#include "CCollider.h"
#include "CColliderMesh.h"
#include "CModel.h"

class CMoveFloor :public CCharacter {

public:
	//デフォルトコンストラクタ
	CMoveFloor();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡縮</param>
	CMoveFloor(CVector& pos,
		CVector& rot, CVector& scale);
	
	//更新
	void Update();

	static CModel* GetModelRedCube();
	static CModel* GetModelBlueCube();
private:
	static CModel mModelRedCube;
	static CModel mModelBuleCube;

	CColliderMesh mColliderMesh;
};

#endif // !CMOVEFLOOR_H

