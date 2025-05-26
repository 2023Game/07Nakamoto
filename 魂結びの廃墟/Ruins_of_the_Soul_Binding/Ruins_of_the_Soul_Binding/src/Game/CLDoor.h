#pragma once
#include "CInteractObject.h"
#include "CColliderMesh.h"

class CLDoor : public CInteractObject
{
public:
	// コンストラクタ
	CLDoor(const CVector& pos,const CVector& rot);
	// デストラクタ
	~CLDoor();

	// 調べる
	void Interact() override;

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	CModel* mpL_Door;	// モデルデータ
	CModel* mpL_DoorCol;// コライダーデータ

	CColliderMesh* mpL_DoorColliderMesh;

	CVector mDefaultPos;
	CVector mMoveVec;
	float mElapsedTime;

	bool mOpen;	// 扉が開いているか
};