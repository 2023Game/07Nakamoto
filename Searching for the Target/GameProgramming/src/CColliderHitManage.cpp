#include "CColliderHitManager.h"

CColliderHitManager::CColliderHitManager()
	:mCol(ECollider::EFLOOR)
{

}

CColliderHitManager* CColliderHitManager::mpInstance = nullptr;

//インスタンスの取得
CColliderHitManager* CColliderHitManager::GetInstance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CColliderHitManager();
	}
	return mpInstance;
}

std::vector<CCollider*> CColliderHitManager::mpHitColliders;

//プレイヤーと当たっているコライダを可変長配列に追加
void CColliderHitManager::AddColliders(CCollider* o)
{
	//既に入ってないか確認
	for (size_t i = 0; i < mpHitColliders.size(); i++)
	{
		if (mpHitColliders[i] == o)
			return;
	}
	//同じアドレスがなければ追加
	mpHitColliders.push_back(o);
}

//コライダーの種類によって判定するコライダーを１つ取り出す
CCollider *CColliderHitManager::HitColliderSerch()
{
	if (!mpHitColliders.size())
	{
		return nullptr;
	}

	//坂の数、床の数
	int s = 0, f = 0;

	for (size_t i = 0; i < mpHitColliders.size(); i++)
	{
		if (mpHitColliders[i]->GetTag() == CCollider::ETag::ESLOPE)
		{
			s++;
		}
		else if (mpHitColliders[i]->GetTag() == CCollider::ETag::EFLOOR)
		{
			f++;
		}
	}

	//坂と床が当たっている数の確認用
	//printf("s:%2d f:%2d\n", s, f);

	if (s > 0 && f == 0)
	{
		mCol = ECollider::ESLOPE;
	}
	else if (s == 0 && f > 0)
	{
		mCol = ECollider::EFLOOR;
	}
	/*
	else if (s > 0 && f > 0 && mCol == ECollider::EFLOOR)
	{
		mCol = ECollider::EFLOOR;
	}
	else if (s > 0 && f > 0 && mCol == ECollider::ESLOPE)
	{
		mCol = ECollider::ESLOPE;
	}
	*/

	//坂のコライダーがあったら
	if (s > 0)
	{
		for (size_t i = 0; i < mpHitColliders.size(); i++)
		{
			if (mpHitColliders[i]->GetTag() == CCollider::ETag::ESLOPE)
			{
				return mpHitColliders[i];
			}
		}
	}
	//床のコライダーしか無かったら
	else if (s == 0 && f > 0)
	{
		for (size_t i = 0; i < mpHitColliders.size(); i++)
		{
			if (mpHitColliders[i]->GetTag() == CCollider::ETag::EFLOOR)
			{
				return mpHitColliders[i];
			}
		}
	}
	
	//両方ない場合
	return mpHitColliders[0];
	
}

float x = 0, y = 180, z = 0;

//衝突判定
void CColliderHitManager::HitCollision(CCollider* m, CCollider* o)
{
	if (o != nullptr)
	{
		//CVector adjust;	//調整値
		//三角形と球の衝突判定
		//if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		//{
		//	m->GetParent()->SetPosition(m->GetParent()->GetPosition() + adjust);
		//}

		//斜面の角度を求め
		CCollider::Slope(m, o, &ajustRote);

		//printf("%10f %10f %10f\n", ajustRote.GetX(), ajustRote.GetY(), ajustRote.GetZ());

		//坂に当たったら斜面に合わせて回転
		m->GetParent()->SetRotation(ajustRote);

		//徐々に回転させる(失敗)
		/*
		if (m->GetParent()->GetRotation().GetX() < ajustRote.GetX())
			x = m->GetParent()->GetRotation().GetX() - 1;
		else if (m->GetParent()->GetRotation().GetX() > ajustRote.GetX())
			x = m->GetParent()->GetRotation().GetX() + 1;

		if (m->GetParent()->GetRotation().GetY() < ajustRote.GetY())
			y = m->GetParent()->GetRotation().GetY() - 1;
		else if (m->GetParent()->GetRotation().GetY() > ajustRote.GetY()+180)
			y = m->GetParent()->GetRotation().GetY() + 1;

		if (m->GetParent()->GetRotation().GetZ() < ajustRote.GetZ())
			z = m->GetParent()->GetRotation().GetZ() - 1;
		else if (m->GetParent()->GetRotation().GetZ() > ajustRote.GetZ())
			z = m->GetParent()->GetRotation().GetZ() + 1;

		m->GetParent()->SetRotation(CVector(x, y, z));
		*/
	}
}

//修正値の取得
CVector CColliderHitManager::GetAjustRote()
{
	return ajustRote;
}

//処理するコライダの種類の取得
CColliderHitManager::ECollider CColliderHitManager::GetCol()
{
	return mCol;
}

//削除
void CColliderHitManager::Delete()
{
	mpHitColliders.clear();
}
