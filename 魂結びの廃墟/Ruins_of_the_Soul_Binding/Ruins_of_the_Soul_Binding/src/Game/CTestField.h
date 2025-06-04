#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CRDoor;
class CLDoor;

class CTestField : public CObjectBase
{
public:
	// フィールドのインスタンスを返す
	static CTestField* Instance();

	CTestField();
	~CTestField();

	// 床のコライダーを取得
	CColliderMesh* GetFloorCol() const;
	// 壁のコライダーの取得
	CColliderMesh* GetWallCol() const;

	/// <summary>
	/// レイとフィールドオブジェクトの衝突判定
	/// </summary>
	/// <param name="start">レイの開始位置</param>
	/// <param name="end">レイの終了位置</param>
	/// <param name="hit">衝突情報返却用</param>
	/// <returns>衝突したら、trueを返す</returns>
	bool CollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit) override;

	void Update() override;
	void Render() override;

private:
	// 経路探索用のノードを作成
	void CreateNavNodes();

	void CreateFieldObjects();

	// フィールドのインスタンス
	static CTestField* spInstance;

	EScene mScene;

	CModel* mpFloor;
	CModel* mpWall;
	CModel* mpWallCol;

	CRDoor* mpRDoor;
	CLDoor* mpLDoor;

	CColliderMesh* mpFloorColliderMesh;
	CColliderMesh* mpWallColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;
};