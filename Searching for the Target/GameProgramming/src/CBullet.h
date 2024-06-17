#ifndef CBULLET_H
#define CBULLET_H

#include "CCharacter.h"
#include "CCollider.h"
#include "CModel.h"
#include "CColliderMesh.h"

/*
弾クラス
三角形を飛ばす
*/
class CBullet : public CCharacter {
public:
	//デフォルトコンストラクタ
	CBullet();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="m">コライダ1</param>
	/// <param name="o">コライダ2</param>
	void Collision(CCollider* m, CCollider* o);
	
	//更新
	void Update();

	//衝突処理
	void Collision();

	static CModel* GetModelBullet();
private:
	static CModel mModelBullet;

	//生存時間
	int mLife;
	//球コライダ
	CCollider mCollider;

	CColliderMesh mMesh;
};

#endif // !CBULLET_H
