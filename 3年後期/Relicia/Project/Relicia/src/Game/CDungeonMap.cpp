#include "CDungeonMap.h"
#include "Maths.h"

// 生成する部屋の幅と高さの最小値
#define MIN_ROOM_SIZE 5

// 生成する部屋の幅と奥行の最大値
// 区画サイズを-2して両端の列を空けた部屋の最大値を設定する
// 区画サイズが偶数の場合は奇数にするために-3をして部屋の最大値とする
#define MAX_ROOM_WIDTH (ROOM_WIDTH % 2 == 0 ? ROOM_WIDTH - 3 : ROOM_WIDTH - 2)
#define MAX_ROOM_HEIGHT (ROOM_HEIGHT % 2 == 0 ? ROOM_HEIGHT - 3 : ROOM_HEIGHT -2)

// 部屋の扉の最大数
#define MAX_DOOR 3

// コンストラクタ
CDungeonMap::CDungeonMap()
{
	// 区画の初期化
	Initialize(ROOM_HEIGHT, ROOM_WIDTH);
	// 扉に変更する候補格納用リストの初期化
	mEntranceCandidates.clear();

	// 区画のデータの宣言
	RoomInfo info;
	SetRoomParameters(info);

	// 部屋の初期化
	InitializeSection();
	// ①部屋の床の設定
	CreateRoomFloor(info);
	// ②部屋の壁の設定
	CreateRoomWall(info);
	// ③部屋の四隅の柱を設定
	CreateRoomPillar(info);
	// 設定した部屋をリストに格納
	mRooms.push_back(info);

	// ④部屋の出入口の設定
	//CreateRoomEntrance(info);

#if _DEBUG
	// 区画タイルのデバッグ表示
	PrintSection();
#endif
}

// デストラクタ
CDungeonMap::~CDungeonMap()
{
}

// 区画リストの初期化
void CDungeonMap::Initialize(int width, int height)
{
	mMapData.resize(height);
	for (int y = 0; y < height; ++y)
	{
		// デフォルトTileで初期化
		mMapData[y].resize(width, Tile()); 
	}
}

// タイルの情報を取得
const CDungeonMap::Tile& CDungeonMap::GetTile(int x, int y) const
{
	return mMapData[y][x];
}

// 区画内の部屋リストを取得
std::vector<CDungeonMap::RoomInfo> CDungeonMap::GetRooms() const
{
	return mRooms;
}

// 部屋の出入口のリストを取得
std::vector<CDungeonMap::Point> CDungeonMap::GetEntrances() const
{
	return mEntrances;
}

// タイルタイプの設定
void CDungeonMap::SetTileType(int x, int y, TileType type)
{
	if (y >= 0 && y < mMapData.size() && x >= 0 && x < mMapData[y].size())
		mMapData[y][x].typeId = type;
}

// 部屋の出入口の設定(出入口を作る方角を指定)
void CDungeonMap::CreateRoomEntrance(Direction dir)
{
	const RoomInfo& info = mRooms[0];

	if (dir == Direction::eSouth)
	{
		// 部屋の上端の壁（y座標が大きい方）
		int index = Math::Rand(info.x + 2, info.x + info.w - 3);
		mMapData[info.y][index].typeId = TileType::eEntrance;
	}
	else if (dir == Direction::eNorth)
	{
		// 部屋の下端の壁（y座標が小さい方）
		int index = Math::Rand(info.x + 2, info.x + info.w - 3);
		mMapData[info.y + info.h - 1][index].typeId = TileType::eEntrance;
	}
	else if (dir == Direction::eEast)
	{
		// 部屋の右端の壁(x座標が大きい方)
		int index = Math::Rand(info.y + 2, info.y + info.h - 3);
		mMapData[index][info.x].typeId = TileType::eEntrance;
	}
	else if (dir == Direction::eWest)
	{
		// 部屋の左端の壁 (x座標が小さい方)
		int index = Math::Rand(info.y + 2, info.y + info.h - 3);
		mMapData[index][info.x + info.w - 1].typeId = TileType::eEntrance;
	}
}


// 部屋の情報を設定
void CDungeonMap::SetRoomParameters(RoomInfo& info)
{
	// 部屋の幅を設定
	info.w = Math::Rand(MIN_ROOM_SIZE, MAX_ROOM_WIDTH);
	// もし、偶数の場合、強制的に奇数にする
	if (info.w % 2 == 0) info.w++;

	// 部屋の奥行を設定;
	info.h = Math::Rand(MIN_ROOM_SIZE, MAX_ROOM_HEIGHT);
	// もし、偶数の場合、強制的に奇数にする
	if (info.h % 2 == 0) info.h++;

	// 配置できる最大座標を設定
	int maxX = ROOM_WIDTH - info.w - 1;
	int maxY = ROOM_HEIGHT - info.h - 1;

	// 部屋を配置する開始座標 (左上隅) をランダムに決定
	info.x = Math::Rand(1, maxX);
	info.y = Math::Rand(1, maxY);

	// 配置開始座標も奇数に強制する
	if (info.x % 2 == 0) info.x++;
	if (info.y % 2 == 0) info.y++;

	// info.x/Yを調整した結果、maxX/maxYをわずかに超えてしまう可能性があるため、
	// 越えた場合は、2減らして(奇数を保って)範囲内に収める
	if (info.x > maxX) info.x -= 2;
	if (info.y > maxY) info.y -= 2;
}

// 区画の初期化
void CDungeonMap::InitializeSection()
{
	for (int y = 0; y < ROOM_HEIGHT; y++)
	{
		for (int x = 0; x < ROOM_WIDTH; x++)
		{
			mMapData[y][x].typeId = TileType::None;
		}
	}
}

// 部屋の床の設定
void CDungeonMap::CreateRoomFloor(const RoomInfo& info)
{
	// 床を設定
	for (int y = info.y + 1; y < info.y + info.h - 1; y++)
	{
		for (int x = info.x + 1; x < info.x + info.w - 1; x++)
		{
			mMapData[y][x].typeId = TileType::eFloor;
		}
	}
}

// 部屋の壁の設定
void CDungeonMap::CreateRoomWall(const RoomInfo& info)
{
	// 上下の壁の設定
	for (int x = info.x + 1; x < info.x + info.w - 1; x++)
	{
		// 上の壁
		mMapData[info.y][x].typeId = TileType::eWall;
		mMapData[info.y][x].dir = Direction::eSouth;

		// 下の壁
		mMapData[info.y + info.h - 1][x].typeId = TileType::eWall;
		mMapData[info.y + info.h - 1][x].dir = Direction::eNorth;

		// もしxの値が奇数なら
		if (x % 2 == 1)
		{
			// 上の壁の座標を追加
			mEntranceCandidates.push_back({ x, info.y });

			// 下の壁の座標を追加
			mEntranceCandidates.push_back({ x, info.y + info.h - 1 });
		}
	}
	// 左右の壁を設定
	for (int y = info.y + 1; y < info.y + info.h - 1; y++)
	{
		// 左の壁
		mMapData[y][info.x].typeId = TileType::eWall;
		mMapData[y][info.x].dir = Direction::eEast;

		// 右の壁
		mMapData[y][info.x + info.w - 1].typeId = TileType::eWall;
		mMapData[y][info.x + info.w - 1].dir = Direction::eWest;

		// もしyの値が奇数なら
		if (y % 2 == 1)
		{
			// 左の壁の座標を追加
			mEntranceCandidates.push_back({ info.x, y });

			// 右の壁の座標を追加
			mEntranceCandidates.push_back({ info.x + info.w - 1, y });
		}
	}
}

// 部屋の四隅の柱を設定
void CDungeonMap::CreateRoomPillar(const RoomInfo& info)
{
	mMapData[info.y][info.x].typeId = TileType::ePillar;	// 左上
	mMapData[info.y][info.x].dir = Direction::eNorthWest;	// 北西
	mMapData[info.y][info.x + info.w - 1].typeId = TileType::ePillar;	// 右上
	mMapData[info.y][info.x + info.w - 1].dir = Direction::eNorthEast;	// 北東
	mMapData[info.y + info.h - 1][info.x].typeId = TileType::ePillar;	// 左下
	mMapData[info.y + info.h - 1][info.x].dir = Direction::eSouthWest;	// 南西
	mMapData[info.y + info.h - 1][info.x + info.w - 1].typeId = TileType::ePillar;	// 右下
	mMapData[info.y + info.h - 1][info.x + info.w - 1].dir = Direction::eSouthEast;	// 南東
}


// 部屋の出入口の設定
void CDungeonMap::CreateRoomEntrance(const RoomInfo& info)
{
	// 部屋の出入口の数を設定
	int numEntrances = Math::Rand(1, MAX_DOOR);

	// 候補リストのサイズを取得
	int currentSize = static_cast<int>(mEntranceCandidates.size());
	// 部屋が小さすぎて出入口の候補数が少ない場合に対応
	numEntrances = std::min(numEntrances, currentSize);

	// 生成する出入口が無くなるまで
	while (numEntrances > 0)
	{
		// 0から最大インデックスまでの乱数を生成
		int randomIndex = Math::Rand(0, currentSize - 1);
		// ランダムに選ばれた座標を取得
		Point selectedDoorPos = mEntranceCandidates[randomIndex];
		// その位置のタイルを出入口に上書き
		mMapData[selectedDoorPos.y][selectedDoorPos.x].typeId = TileType::eEntrance;

		// 出入口に変更したデータをリストに追加
		mEntrances.push_back(selectedDoorPos);

		// 壁から出入口に上書きした要素をリストの末尾と入れ替える
		mEntranceCandidates[randomIndex] = mEntranceCandidates.back();
		// 末尾の要素を削除する
		mEntranceCandidates.pop_back();
		// リストのサイズを更新
		currentSize--;
		// 生成する出入口のカウントを減らす
		numEntrances--;
	}
}

#if _DEBUG
// 区画のタイルのデバック表示
void CDungeonMap::PrintSection()
{
	for (int y = 0; y < ROOM_HEIGHT; y++)
	{
		for (int x = 0; x < ROOM_WIDTH; x++)
		{
			if (mMapData[y][x].typeId == TileType::None) printf("0");
			else if (mMapData[y][x].typeId == TileType::eFloor) printf("1");
			else if (mMapData[y][x].typeId == TileType::eWall) printf("2");
			else if (mMapData[y][x].typeId == TileType::ePillar) printf("3");
			else if (mMapData[y][x].typeId == TileType::eEntrance) printf("4");
		}
		printf("\n");
	}
}
#endif