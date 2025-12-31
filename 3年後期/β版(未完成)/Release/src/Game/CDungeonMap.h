#pragma once
#include "CDungeonManeger.h"


#define ROOM_WIDTH 15	// １区画の幅
#define ROOM_HEIGHT 15	// １区画の奥行

class CDungeonMap
{
public:
	// コンストラクタ
	CDungeonMap();
	// デストラクタ
	~CDungeonMap();

	// 方向を表すデータ
	enum class Direction
	{
		eNorth,	// 北
		eSouth,	// 南
		eEast,	// 東
		eWest,	// 西

		eNorthEast,	// 北東
		eSouthEast,	// 南東
		eSouthWest,	// 南西
		eNorthWest,	// 北西
	};

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
	};

	// タイルの情報
	struct Tile
	{
		TileType typeId = TileType::None;	// タイルの種類
		Direction dir = Direction::eNorth;	// 方向(壁や扉の回転用)
	};

	// 区画のデータ
	struct RoomInfo
	{
		// マップのインデックス(左上隅の座標)
		int x;
		int y;

		// 部屋のサイズ(幅と奥行)
		int w;
		int h;
	};

	// 壁の配列のデータ
	struct Point
	{
		int x, y;
	};

	// タイルの情報を取得
	const Tile& GetTile(int x,int y) const;

	// 区画内の部屋リストを取得
	std::vector<RoomInfo> GetRooms() const;
	// 部屋の出入口のリストを取得
	std::vector<Point> GetEntrances() const;

	// タイルタイプの設定
	void SetTileType(int x, int y, TileType type);
	// 部屋の出入口の設定(出入口を作る方角を指定)
	void CreateRoomEntrance(Direction dir);

	std::vector<Point> GetmEntrances();

private:
	// 区画内の部屋リスト
	std::vector<RoomInfo> mRooms;
	// 区画のデータ配列
	std::vector<std::vector<Tile>> mMapData;

	// 出入口に変更する候補格納用リスト
	std::vector<Point> mEntranceCandidates;
	// 部屋の出入口を保存するためのリスト
	std::vector<Point> mEntrances;

	// 区画リストの初期化
	void Initialize(int width, int height);

	// 部屋の情報を設定
	void SetRoomParameters(RoomInfo& info);
	// 部屋の床の設定
	void CreateRoomFloor(const RoomInfo& info);
	// 部屋の壁の設定
	void CreateRoomWall(const RoomInfo& info);
	// 部屋の四隅の柱を設定
	void CreateRoomPillar(const RoomInfo& info);

	// 出入口同士を繋げる
	void ConnectPassage(CDungeonMap* a, CDungeonMap* b);

	// 部屋の出入口の設定
	void CreateRoomEntrance(const RoomInfo& info);

#if _DEBUG
	// 1区画のタイルのデバック表示
	void PrintSection();
#endif
};