#pragma once

#define TILE_SIZE 20.0f	// タイルモデルの大きさ

class CBspMap2
{
public:
	// コンストラクタ
	CBspMap2(int floorNum);
	// デストラクタ
	~CBspMap2();

	// タイルの種類
	enum class TileType
	{
		None,		// 無し
		eFloor,		// 床

		eBoundary   // 区画の境界線
	};

    // 壁の種類
    enum class EdgeType
    {
        None,       // 何もない（床と床の境界）
        eWall,       // 壁
        eEntrance    // 出入口
    };

    // タイルの情報
    struct Tile
    {
		TileType type = TileType::None;      // タイルの種類
        bool passage = false;       // 通路判定（BSP用）
        // 壁の情報
        EdgeType north = EdgeType::None;     // 北
        EdgeType south = EdgeType::None;     // 南
        EdgeType east = EdgeType::None;      // 東
        EdgeType west = EdgeType::None;      // 西

        bool hasPillar = false;     // 柱があるか
    };

	// 部屋の情報
	struct Room
	{
		int x = 0, y = 0;	// 部屋の左上の柱の座標
		int w = 0, h = 0;	// 部屋のサイズ
		CVector2 center{ 0.0f,0.0f };	// 部屋の中心座標
	};

	// 区画ノード
	struct SectionNode
	{
		int x = 0, y = 0;			// 区画の左上座標
		int width = 0, height = 0;	// 区画サイズ

		SectionNode* left = nullptr;	// 左（または上）子ノード
		SectionNode* right = nullptr;	// 右（または下）子ノード

		Room room;				// 部屋の情報
		CVector2 connectPoint = CVector2(0.0f, 0.0f);	// 通路接続用の代表点

		bool hasRoom = false;	// 部屋があるか
	};
	
	// 階層に応じた初期化
	void InitializeForFloor(int floor);

private:
	// 区画の大きさ
	struct FloorSetting
	{
		int mapWidth;
		int mapHeight;

		int minSectionSize;
		int minRoomSize;

		int maxDepth;      // BSP分割の深さ
	};
	// 階層ごとの区画の大きさを格納
	static const FloorSetting mFloorSettings[];

	// 初期化
	void Initialize(int width, int height);
	// ノードの削除
	void DeleteSection(SectionNode* node);

	// 区画分け
	void Split(SectionNode* node, int depth);
	// 部屋の床を生成
	void CreateRoom(SectionNode* node);
	// 壁の生成
	void BuildWalls();
	// 通路の生成
	void CreateCorridor(SectionNode* node);
	// 通路を掘る
	void DigCorridor(CVector2 from, CVector2 to);

	int mMinSectionSize;	// 区画の最小サイズ
	int mMapWidth;			// 区画の横幅
	int mMapHeight;			// 区画の縦幅
	int mMinRoomSize;		// 部屋の最小サイズ
	int mMaxDepth;			// 区画の最大分割数

	// ルートノード(最初の区画)
	SectionNode* mpRoot;
	// ２次元配列(可変長配列)のマップデータ
	std::vector<std::vector<Tile>> mMapData;

#if _DEBUG
	// 葉の区画の大きさの表示		
	void PrintSection(const SectionNode* node, int depth = 0);
	// 部屋の床の表示
	void DebugPrintFloor() const;
	// ダンジョンの表示
	void DebugPrintDungion() const;
#endif
};