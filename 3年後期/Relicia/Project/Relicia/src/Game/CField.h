#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CDungeonMap.h"
#include "CBspMap.h"

class CFloor;
class CWall;
class CPillar;
class CEntrance;
class CDoor;
class CCrystalObj;
class CItemObj;
class CCollider;
class CBspMapCollider;
class CEscapeArea;

class CField : public CObjectBase
{
public:
	// フィールドのインスタンスを返す
	static CField* Instance();

	// コンストラクタ
	CField();
	// デストラクタ
	~CField();

	// 2次元配列のダンジョンデータを取得
	CBspMap* GetMapData() const;
	
	// 床タイルのランダムな座標を取得
	//const CVector GetRandomFloorPos() const;
	
	// ダンジョンマップの生成
	void CreateMap();

	// 床コライダーを取得
	CColliderMesh* GetFloorCollider() const;
	// 壁コライダーを取得
	CColliderMesh* GetWallCollider() const;
	// フィールドのオブジェクトのコライダーを取得
	std::vector<CCollider*> GetObjectCollider() const;
	// フィールドのオブジェクトのコライダーを追加
	void AddObjectCollider(CCollider* col);

	// 徘徊用のノードを取得
	std::vector<CNavNode*> GetNavNodes();

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

	void CreateFieldObjects();

	// 方角によって回転値を設定
	int ConvertDirectionAngle(CBspMap::Direction dir) const;
	// 方角によってオフセット座標を設定
	CVector2 CvnvertDirectionPos(CBspMap::Direction dir) const;

	// BPMマップのダンジョンの生成
	void CreateDungeon(const std::vector<std::vector<CBspMap::Tile>>& map);

	// 通路の壁の生成
	void CreatePassageWall(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y);
	// 通路の柱を生成
	void CreatePassagePillar(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y);

	// 通路タイルの接続数を数える
	int CountPassageConnections(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y);
	// 曲がり角かどうかを判定
	bool IsCorner(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y);
	// 斜めの通路の数
	bool IsInsideWidePassage(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y);

	// 出入口があるかどうか
	//bool IsWidePassageEntrance(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y);
	
	// ノード配置ルール
	void TryAddNavNode(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y);

	// フィールドのインスタンス
	static CField* spInstance;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;
	
	// ２次元配列のマップデータ保存用
	CBspMap* mpMapData;

	// それぞれの3Dオブジェクトのリスト
	std::vector<CFloor*> mpFloorObjects;	// 床のリスト
	std::vector<CWall*> mpWallObjects;		// 壁のリスト
	std::vector<CPillar*> mpPillarObjects;	// 柱のリスト
	std::vector<CEntrance*> mpEntranceObjects;	// 出入口のリスト
	std::vector<CDoor*> mpDoorObjects;		// 扉のリスト
	std::vector<CFloor*> mpPassegeObjects;	// 通路の床リスト
	std::vector<CVector> mNavNodePositions;	// NavNodeを配置する座標のリスト
	std::vector<CNavNode*> mpNavNodes;		// NavNodeリスト
	std::vector<CObjectBase*> mpObjects;	// 遮蔽物になるオブジェクトのリスト
	std::vector<CCollider*> mpObjectColliders;	// 遮蔽になるオブジェクトのコライダーリスト

	CEscapeArea* mpEscapeArea;	// 脱出エリア

	// コライダーのポインタ
	CBspMapCollider* mpDungeonCollider;

};