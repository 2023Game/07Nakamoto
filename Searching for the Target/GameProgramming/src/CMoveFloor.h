#ifndef CMOVEFLOOR_H
#define CMOVEFLOOR_H

#include "CCharacter.h"
#include "CColliderMesh.h"
#include "CModel.h"
#include "CColliderLine.h"

class CMoveFloor :public CCharacter {

public:
	//デフォルトコンストラクタ
	CMoveFloor();

	/// <summary>
	/// 動く床の設定
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡縮</param>
	void SetMoveFloor(const CVector& position,
		const CVector& rotation, const CVector& scale);

	//動く床のモデルの取得
	static CModel* GetModelFloor();
	
	//更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="m">自分のコライダ</param>
	/// <param name="o">相手のコライダ</param>
	void Collision(CCollider* m, CCollider* o);

	//衝突処理
	void Collision();

private:
	//モデルを格納
	static CModel mModelFloor;
	//移動距離
	float mMoveDistance;
	//移動した距離
	float mMove;

	//コライダ
	CColliderMesh mColliderMesh;
	//線コライダ
	CColliderLine mCLine;

};

#endif // !CMOVEFLOOR_H

