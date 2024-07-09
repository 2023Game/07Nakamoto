#include "CXEnemy.h"

//デフォルトコンストラクタ
CXEnemy::CXEnemy()
	: mColShereBody(this, nullptr, CVector(), 0.5f)
	, mColShereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColShereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
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