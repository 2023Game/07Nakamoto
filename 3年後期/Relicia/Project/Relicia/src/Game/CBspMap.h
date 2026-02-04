#pragma once
#include "CObjectBase.h"

#define TILE_SIZE 20.0f	// タイルモデルの大きさ

class CBspMap 
{
public:
	// コンストラクタ
	CBspMap(int x,int y);
	// デストラクタ
	~CBspMap();

	// タイルの種類
	enum class TileType
	{
		None,		// 無し
		eFloor,		// 床
		eWall,		// 壁
		eEntrance,	// 出入口
		ePillar,	// 柱
		ePassage,	// 通路
		eDoor,		// 扉

		eBoundary   // 区画の境界線
	};

	// 方向を表すデータ
	enum class Direction
	{
		None,

		eNorth,	// 北(y-1の方向)
		eSouth,	// 南(y+1の方向)
		eEast,	// 東(x+1の方向)
		eWest,	// 西(x-1の方向)

		eNorthEast,	// 北東
		eSouthEast,	// 南東
		eSouthWest,	// 南西
		eNorthWest,	// 北西
	};

	// タイルの情報
	struct Tile
	{
		TileType type = TileType::None;	// タイルの種類
		Direction dir = Direction::None;	// 向き
		bool passage = false;	// 通路かどうか
		Direction pillar = Direction::None;	// 柱を生成するかどうか 
		Direction passageDir = Direction::None;	// 通路の向き

		bool occupy = false;	// タイルが占有されているか
	};

	// 部屋の情報
	struct Room
	{
		int x = 0, y = 0;			// 部屋の左上の柱の座標
		int width = 0, height = 0;	// 部屋のサイズ
		CVector2 center{ 0.0f,0.0f };	// 部屋の中心座標
		
		// 部屋の種類
		enum class RoomType
		{
			eNormal,	// 通常部屋
			eBoss,		// ボス部屋
			eEntrance,	// 開始部屋
			eExit,		// 脱出部屋
		};

		RoomType type = RoomType::eNormal;

		bool connected = false;	// 繋がっているか
	};

	// 区画ノード
	struct SectionNode
	{
		int x = 0, y = 0;			// 区画の左上座標
		int width = 0, height = 0;	// 区画サイズ

		SectionNode* left = nullptr;	// 左（または上）子ノード
		SectionNode* right = nullptr;	// 右（または下）子ノード

		Room room{}; // 部屋データ
	};

	// 壁の線分情報を格納する構造体
	struct TileSegment
	{
		CVector2 start{ 0.0f,0.0f };	// 壁のスタート座標
		CVector2 end{ 0.0f,0.0f };	// 壁の終了座標
		TileType type = TileType::None;	// タイルの種類
		Direction dir = Direction::None;	// 向き
	};

	// BSPマップデータの取得
	const std::vector<std::vector<Tile>>& GetTileData()const;
	// ルートノードの取得
	const SectionNode* GetRootNode()const;
	// 通路と部屋の壁の開始座標と終了座標のリストを取得
	std::vector<TileSegment> GetWallSegments() const;
	// マップデータを走査して経路探索用のノードを設定
	//void SetNavNodes();

	// 部屋の壁の情報を返す
	//std::vector<TileSegment> CBspMap::CollectWallSegments() const;

	// 占有管理用のタイプ
	enum class EOccupyType
	{
		None,
		Player,
		Enemy,
		Item,
		Crystal,
		Object,
	};

	// 指定したタイル座標(x, y)がすでに他オブジェクトにより
	// 占有されているかを判定する（スポーン判定用）
	bool IsOccupied(int x, int y);
	// 指定したタイル座標(x, y)を占有状態に設定する
	// type : 占有しているオブジェクトの種別
	void SetOccupied(int x, int y, EOccupyType type);
	// 指定したワールド座標を中心として、
	// オブジェクトが占有するタイル範囲（例: 2×2）が
	// すべて未占有であるかを判定する
	// true  : 配置可能
	// false : すでに他オブジェクトが存在する
	bool CanPlaceObject(float worldX, float worldZ);
	// 指定したワールド座標を中心として、
	// オブジェクトが使用するタイル範囲（例: 2×2）を
	// 占有状態として登録する
	void OccupyObject(float worldX, float worldZ, EOccupyType type);
	// 占有の共用タイプを取得
	bool IsBlocking(EOccupyType type);
	// 占有状態のリセット
	void ResetOccupy();

	// 部屋の床の座標のリストからランダムに座標を取得
	CVector GetRoomRandomFloorPos(EOccupyType occupyType);

private:
	// ノードの削除
	void DeleteNode(SectionNode* node);

	// 初期化
	void Initialize(int width, int height);

	// 区画分け
	void Split(SectionNode* node);

	// 区画ごとに部屋の設定
	void PlaceRoom(SectionNode* node, std::vector<std::vector<Tile>>& map);

	// 部屋の床データの設定
	void CreateRoomFloor(SectionNode* node, std::vector<std::vector<Tile>>& map);
	// 部屋の壁データの設定
	void CreateRoomWall(const Room& room, std::vector<std::vector<Tile>>& map);
	// 部屋の四隅データの柱を設定
	void CreateRoomPillar(const Room& room, std::vector<std::vector<Tile>>& map);

	// 部屋の床をリストに保存
	void RoomFloorData(std::vector<std::vector<Tile>>& map);

	// 同じ階層の部屋同士を通路で繋げる
	void ConnectRooms(SectionNode* node, std::vector<std::vector<Tile>>& map);

	// 部屋のランダムな座標を取得
	CVector2 GetRoomRandomPos(SectionNode* node);

	// 部屋同士の通路データの設定
	void CreatePassage(std::vector<std::vector<Tile>>& map, SectionNode* nodeA, SectionNode* nodeb);

	// 方角の正反対を返す
	Direction InverseDirection(Direction dir) const;

#if _DEBUG
	// 区画の境界線設定
	void DrawBoundary(SectionNode* node, std::vector<std::vector<Tile>>& map);
	// 2次元配列のデバッグ表示
	void PrintSection();
#endif // _DEFU

	// ルートノード(最初の区画)
	SectionNode* mpRoot;
	// ２次元配列(可変長配列)のマップデータ
	std::vector<std::vector<Tile>> mMapData;

	// 部屋の床データのリスト
	std::vector<CVector2> mpRoomFloors;
	// 占有管理用の2次元配列
	std::vector<std::vector<EOccupyType>> mOccupyMap;

	float mTimer;	// 経過時間
};