#ifndef CMOVINGDOOR_H
#define CMOVINGDOOR_H

#include "CCharacter.h"
#include "CColliderMesh.h"
#include "CModel.h"
#include "CSwitch.h"

class CMovingDoor : public CCharacter
{
public:

	//デフォルトコンストラクタ
	CMovingDoor();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親クラス</param>
	/// <param name="position">位置</param>
	/// <param name="rotation">回転</param>
	/// <param name="scale">拡縮</param>
	CMovingDoor(CSwitch* parent, const CVector& position,
		const CVector& rotation, const CVector& scale);

	void SetMovingDoor(CSwitch* parent, const CVector& position,
		const CVector& rotation, const CVector& scale);

	//更新処理
	void Update();

	//モデルの取得
	static CModel* GetModelCube();

private:
	//スイッチ(親)
	CSwitch* mpSwhitch;
	//モデルを格納
	static CModel mModelCube;
	//コライダ
	CColliderMesh mColliderMesh;
	//移動距離
	float mMoveDistance;
	//移動した距離
	float mMove;
};

#endif // !CMOVINGDOOR_H

