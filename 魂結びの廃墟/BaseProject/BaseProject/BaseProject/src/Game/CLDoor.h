#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CLDoor : public CObjectBase
{
public:
	// コンストラクタ
	CLDoor();
	// デストラクタ
	~CLDoor();

	// 更新処理
	void Update();
	// 描画処理
	void Render();

private:
	CModel* mpL_Door;
	CModel* mpL_DoorCol;

	CColliderMesh* mpL_DoorColliderMesh;

};