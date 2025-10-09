#pragma once

#define ROOM_WIDTH 15	// 区画の幅
#define ROOM_HEIGHT 15	// 区画の奥行

#define TILE_SIZE 20.0f	// タイルモデルの大きさ

class CDangeonMap
{
public:
	// コンストラクタ
	CDangeonMap();
	// デストラクタ
	~CDangeonMap();

	// 方向を表すデータ
	enum class Direction
	{
		eNorth,	// 北
		eSouth,	// 南
		eEast,	// 東
		eWest,	// 西

	};

	// タイルの種類
	enum class TileType
	{
		None,		// 無し
		eFloor,		// 床
		eWall,		// 壁
		eEntrance,	// 出入口
		eDoor,		// 扉
		ePillar,	// 柱
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
private:
	// 部屋の情報を設定
	void SetRoomParameters(RoomInfo& info);

	// 区画のデータ配列
	Tile mMapData[ROOM_HEIGHT][ROOM_WIDTH];
	// 扉に変更する候補格納用リスト
	std::vector<Point> mDoorCandidates;

	// 区画の初期化
	void InitializeSection();

	// 部屋の床の設定
	void CreateRoomFloor(const RoomInfo& info);
	// 部屋の壁の設定
	void CreateRoomWall(const RoomInfo& info);
	// 部屋の四隅の柱を設定
	void CreateRoomPillar(const RoomInfo& info);
	// 部屋の扉の設定
	void CreateRoomDoor(const RoomInfo& info);

#if _DEBUG
	// 区画のタイルのデバック表示
	void PrintSection();
#endif
};