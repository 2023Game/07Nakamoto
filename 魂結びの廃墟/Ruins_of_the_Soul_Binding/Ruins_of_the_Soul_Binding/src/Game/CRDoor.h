#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CRDoor : public CObjectBase
{
public:
	// コンストラクタ
	CRDoor();
	// デストラクタ
	~CRDoor();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	CModel* mpR_Door;
	CModel* mpR_DoorCol;

	CColliderMesh* mpR_DoorColliderMesh;
};