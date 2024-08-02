#ifndef CMOVEWALL_H
#define CMOVEWALL_H

#include "CCharacter.h"
#include "CColliderMesh.h"
#include "CModel.h"
#include "CSwitch.h"

class CMoveWall : public CCharacter
{
public:

	//コンストラクタ
	CMoveWall(CSwitch* parent, const CVector& position,
		const CVector& rotation, const CVector& scale);

	//更新処理
	void Update();

	//動く壁のモデルの取得
	static CModel* GetModelWall();

	//衝突処理
	//void Collision();

private:
	//スイッチ(親)
	CSwitch* mpSwhitch;
	//モデルを格納
	static CModel mModelWall;
	//コライダ
	CColliderMesh mColliderMesh;
	//CCollider mCollider;
	//移動距離
	float mMoveDistance;
	//移動した距離
	float mMove;
};

#endif // !CMOVINGDOOR_H

