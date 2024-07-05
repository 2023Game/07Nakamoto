#include "CXEnemy.h"

//デフォルトコンストラクタ
CXEnemy::CXEnemy()
	: mColShereBody(this, nullptr, CVector(), 0.5f)
	, mColShereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColShereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f)
{
	
}

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//体
	mColShereBody.Matrix(&mpCombinedMatrix[8]);
	//頭
	mColShereHead.Matrix(&mpCombinedMatrix[11]);
	//剣
	mColShereSword.Matrix(&mpCombinedMatrix[21]);
}