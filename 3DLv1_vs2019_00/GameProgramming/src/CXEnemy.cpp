#include "CXEnemy.h"

//デフォルトコンストラクタ
CXEnemy::CXEnemy()
	: mColShereBody(this, nullptr, CVector(0.5f, -1.0f, 0.0f), 1.0f)
	, mColShereHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)
	, mColShereSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f)
	, mColShereSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f)
	, mColShereSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f)
{
	
}

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//体
	mColShereBody.Matrix(&mpCombinedMatrix[1]);
	//頭
	mColShereHead.Matrix(&mpCombinedMatrix[1]);
	//剣
	mColShereSword0.Matrix(&mpCombinedMatrix[26]);
	mColShereSword1.Matrix(&mpCombinedMatrix[26]);
	mColShereSword2.Matrix(&mpCombinedMatrix[26]);
}

void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	switch (m->Type())
	{
	case CCollider::EType::ESPHERE:

		//相手のコライダータイプが球で
		if (o->Type() == CCollider::EType::ESPHERE)
		{
			//相手のコライダのタグが剣で
			if (o->Tag() == CCollider::ETag::ESWORD)
			{
				//自分のコライダのタグが体だったら
				if (m->Tag() == CCollider::ETag::EBODY)
				{
					//衝突しているか判定
					if (CCollider::Collision(m, o))
					{
						//30フレームかけてダウンし、繰り返さない
						ChangeAnimation(11, false, 30);
					}
				}
			}
		}
		break;
	}
}