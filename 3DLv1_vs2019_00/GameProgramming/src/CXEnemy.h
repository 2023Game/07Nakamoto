#ifndef CXENEMY_H
#define XCENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy :public CXCharacter
{
public:
	//デフォルトコンストラクタ
	CXEnemy();

	//初期設定
	void Init(CModelX* model);

	//衝突処理
	void Collision(CCollider* m, CCollider* o);
private:
	//コライダの宣言
	CCollider mColShereBody;	//体
	CCollider mColShereHead;	//頭
	CCollider mColShereSword0;	//剣
	CCollider mColShereSword1;	//剣
	CCollider mColShereSword2;	//剣
};

#endif // !CXENEMY_H

