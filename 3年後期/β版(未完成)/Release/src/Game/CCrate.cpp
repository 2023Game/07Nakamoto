#include "CCrate.h"
#include "CColliderMesh.h"

CCrate::CCrate(const CVector& pos)
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
{
	mpModel = CResourceManager::Get<CModel>("Crate");

	mpColliderMesh = new CColliderMesh(this, ELayer::eCrate, mpModel, false);
	mpColliderMesh->SetCollisionTags({ ETag::eField,ETag::ePlayer,ETag::eEnemy });
	mpColliderMesh->SetCollisionLayers({ ELayer::eWall, ELayer::ePlayer,ELayer::eEnemy });

	Position(pos);
}

CCrate::~CCrate()
{
	SAFE_DELETE(mpColliderMesh);
}

// 衝突判定
void CCrate::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderMesh)
	{
		// プレイヤーと衝突した場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// 自身と相手の座標
			CVector selfPos = Position();
			CVector otherPos = other->Owner()->Position();

			//　自身の座標から相手の座標までのベクトルを求める
			CVector vec = otherPos - selfPos;
			vec.Y(0.0f);
			// 正規化して方向ベクトル化
			CVector dir = vec.Normalized();

			// 木箱の法線格納用
			CVector normal = VectorZ();
			// Y軸に90度回転する回転行列を作成
			CMatrix rotMtx;
			rotMtx.RotateY(90.0f);

			// 4方向のベクトルと、相手までのベクトルの内積で角度を求め、
			// 一番近い方向から押し出しベクトルを求める
			float maxDot = -2.0f;
			CVector pushDir = CVector::zero;
			for (int i = 0; i < 4; i++)
			{
				// 内積で角度を求めて、一番近い(内積結果が大きい)方向ベクトルを取得
				float d = CVector::Dot(normal, dir);
				if (d > maxDot)
				{
					pushDir = normal;
					maxDot = d;
				}
				// 調べるベクトルをY軸に90度回転
				normal = rotMtx * normal;
			}

			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 求めた4方向で一番近いベクトル方向に押し戻す
			float length = CVector::Dot(pushDir, adjust * hit.weight);
			adjust = pushDir * length;
			Position(Position() + adjust);
		}
	}
}

void CCrate::Update()
{
}

void CCrate::Render()
{
	mpModel->Render(Matrix());
}
