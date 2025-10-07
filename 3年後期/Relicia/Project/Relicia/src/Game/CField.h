#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

#define ROOM_WIDTH 15
#define ROOM_HEIGHT 15

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;

	// 部屋の大きさを設定
	int mRoom[ROOM_HEIGHT][ROOM_WIDTH];

	// タイルの種類
	enum TileType {

		None = -1,

		eFloor,		// 床
		eWall,		// 壁
		eEntrance,	// 出入口
		eDoor,		// 扉
		ePillar,	// 柱
	};

	// 部屋の初期化
	void InitializeRoom();

	/// <summary>
	/// ダンジョンの生成
	/// </summary>
	/// <param name="sections">区画の数</param>
	void CreateDungeon(int sections);

	// 部屋のタイルのデバック表示
	void PrintRoom();

};