#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CRDoor;
class CLDoor;

class CField : public CObjectBase
{
public:
	// フィールドのインスタンスを返す
	static CField* Instance();

	CField();
	~CField();

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
	
	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// 経路探索用のノードを作成
	void CreateNavNodes();
	// 部屋の作成
	void CreateRoomObjects();
	// ギミックをの生成
	void CreateGimmick();
	// アイテムを生成
	void CreateItem();

	// フィールドのインスタンス
	static CField* spInstance;

	CModel* mpFloor;
	CModel* mpWall;
	CModel* mpWallCol;

	//CModel* mpTestObj;
	//CColliderMesh* mpTestObjWallMesh;
	//CColliderMesh* mpTestObjFloorMesh;

	//CModel* mpTestWall;
	//CColliderMesh* mpTestWallCol;

	CRDoor* mpRDoor;
	CLDoor* mpLDoor;

	CColliderMesh* mpFloorColliderMesh;
	CColliderMesh* mpWallColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;
};