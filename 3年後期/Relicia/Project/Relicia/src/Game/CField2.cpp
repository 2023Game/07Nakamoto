#include "CField2.h"
#include "CBspMap2.h"
#include "CFloor.h"
#include "CWall.h"
#include "CPillar.h"
#include "CEntrance.h"
#include "CDoor.h"

#include "CNavNode.h"
#include "CNavManager.h"

// フィールドのインスタンスを取得
CField2* CField2::Instance()
{
	return spInstance;
}

// コンストラクタ
CField2::CField2(int floorNum)
{
	CreateDungeon();
}

// デストラクタ
CField2::~CField2()
{
}

void CField2::CreateDungeon()
{
	if (mpMapData != nullptr)
	{
		SAFE_DELETE(mpMapData);
		// コライダーの削除
		//SAFE_DELETE(mpDungeonCollider);

		for (CFloor* floor : mpFloorObjects) floor->Kill();
		for (CWall* wall : mpWallObjects) wall->Kill();
		for (CPillar* cpillar : mpPillarObjects) cpillar->Kill();
		for (CEntrance* entrance : mpEntranceObjects) entrance->Kill();
		for (CDoor* door : mpDoorObjects) door->Kill();
		for (CFloor* passegeFloor : mpPassegeObjects) passegeFloor->Kill();
		for (CNavNode* node : mpNavNodes) node->Kill();
		for (CObjectBase* obj : mpObjects) obj->Kill();
		for (CColliderMesh* objcol : mpObjectColliders) objcol = nullptr;

		mpFloorObjects.clear();
		mpWallObjects.clear();
		mpPillarObjects.clear();
		mpEntranceObjects.clear();
		mpDoorObjects.clear();
		mpPassegeObjects.clear();
		mNavNodePositions.clear();
		mpNavNodes.clear();
		mpObjects.clear();
		mpObjectColliders.clear();

		//CNavManager::Instance()->RemoveCollider(GetWallCollider());
		//CItemManager::Instance()->AllRemoveItems();
		//CCrystalManager::Instance()->AllRemoveCrystals();

		// ノード管理クラスをリセット
		CNavManager::Instance()->Clear();
	}

	// ダンジョンの2Dデータを取得 
	mpMapData = new CBspMap2(0);


}

// マップの生成
void CField2::CreateMap()
{
}


