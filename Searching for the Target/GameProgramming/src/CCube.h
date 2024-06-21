#ifndef CCUBE_H
#define CCUBE_H

#include "CCharacter.h"
#include "CColliderMesh.h"
#include "CMoveFloor.h"
#include "CCollider.h"

class CCube :public CCharacter
{
public:
	//デフォルトコンストラクタ
	CCube();

	//更新処理
	void Update();

private:
	CCollider mMesh;
};


#endif // !CCUBE_H

