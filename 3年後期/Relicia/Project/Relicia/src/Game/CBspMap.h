#pragma once

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
		TileType type;	// タイルの種類
		Direction dir;	// 向き
		bool passage;	// 通路かどうか
		bool passageWall;	// 通路の壁を生成したか
	};

	// 部屋の情報
	struct Room
	{
		int x, y;			// 部屋の左上の柱の座標
		int width, height;	// 部屋のサイズ

		// 部屋の種類
		enum class RoomType
		{
			eNormal,	// 通常部屋
			eBoss,		// ボス部屋
			eEntrance,	// 開始部屋
			eExit,		// 脱出部屋
		};

		RoomType type;

		bool connected = false;	// 繋がっているか
	};

	// 区画ノード
	struct SectionNode
	{
		int x, y;			// 区画の左上座標
		int width, height;	// 区画サイズ

		SectionNode* left;	// 左（または上）子ノード
		SectionNode* right;	// 右（または下）子ノード

		Room room; // 部屋データ
	};

	// BSPマップデータの取得
	const std::vector<std::vector<Tile>>& GetTileData()const;
	// 通路の壁を生成フラグをtrueにする
	void SetPassageWall(int x,int y);

	// 部屋の内側で壁に近いか
	bool IsNearRoomWall(const CVector2& pos);

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

	// 同じ階層の部屋同士を通路で繋げる
	void ConnectRooms(SectionNode* node, std::vector<std::vector<Tile>>& map);

	// 部屋のランダムな座標を取得
	CVector2 GetRoomRandomPos(SectionNode* node);

	// 部屋同士の通路データの設定
	void CreatePassage(std::vector<std::vector<Tile>>& map, CVector2 a, CVector2 b);

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
};