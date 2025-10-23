#pragma once

class CBpsMap
{
public:
	// コンストラクタ
	CBpsMap(int x,int y);
	// デストラクタ
	~CBpsMap();

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

	// 部屋の情報
	struct Room
	{
		int x, y;			// 部屋の左上の座標
		int width, height;	// 部屋のサイズ

		TileType type;		// タイルの種類
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

private:
	// ノードの削除
	void DeleteNode(SectionNode* node);

	// 初期化
	void Initialize(int width, int height);

	// 区画分け
	void Split(SectionNode* node);
	// 区画ごとに部屋の設定
	void PlaceRoom(SectionNode* node, std::vector<std::vector<Room>>& map);

#if _DEBUG
	// 2次元配列のデバッグ表示
	void PrintSection();
#endif // _DEFU


	// ルートノード(最初の区画)
	SectionNode* mpRoot;
	// ２次元配列(可変長配列)のマップデータ
	std::vector<std::vector<Room>> mMapData;
};