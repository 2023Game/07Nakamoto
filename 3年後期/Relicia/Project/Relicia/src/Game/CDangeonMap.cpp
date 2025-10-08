#include "CDangeonMap.h"
#include "Maths.h"
#include "CFloor.h"
#include "CDoor.h"
#include "CWall.h"
#include "CPillar.h"
#include "CEntrance.h"

// 生成する部屋の幅と高さの最小値
#define MIN_ROOM_SIZE 3

// 生成する部屋の幅と奥行の最大値
// 区画サイズを-2して両端の列を空けた部屋の最大値を設定する
// 区画サイズが偶数の場合は奇数にするために-3をして部屋の最大値とする
#define MAX_ROOM_WIDTH (ROOM_WIDTH % 2 == 0 ? ROOM_WIDTH - 3 : ROOM_WIDTH - 2)
#define MAX_ROOM_HEIGHT (ROOM_HEIGHT % 2 == 0 ? ROOM_HEIGHT - 3 : ROOM_HEIGHT -2)

#define TILE_SIZE 20.0f	// タイルモデルの大きさ

// コンストラクタ
CDangeonMap::CDangeonMap()
{
	RoomInfo info;

	SetRoomParameters(info);

	// 部屋の初期化
	InitializeSection();

	// 床の設定
	CreateRoomFloor(info);
	// 壁の設定
	CreateRoomWall(info);
	// 四隅の柱を設定
	CreateRoomPillar(info);
	// 扉の設定
	CreateRoomDoor(info);

#if _DEBUG
	// 区画タイルのデバッグ表示
	PrintSection();
#endif
}

// デストラクタ
CDangeonMap::~CDangeonMap()
{
}

// 部屋の情報を設定
void CDangeonMap::SetRoomParameters(RoomInfo& info)
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
void CDangeonMap::InitializeSection()
{
	for (int y = 0; y < ROOM_HEIGHT; y++)
	{
		for (int x = 0; x < ROOM_WIDTH; x++)
		{
			mMapData[y][x].typeId = TileType::None;
		}
	}
}

// 床の設定
void CDangeonMap::CreateRoomFloor(const RoomInfo& info)
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

// 壁の設定
void CDangeonMap::CreateRoomWall(const RoomInfo& info)
{
	// 上下の壁の設定
	for (int x = info.x + 1; x < info.x + info.w - 1; x++)
	{
		mMapData[info.y][x].typeId = TileType::eWall;
		mMapData[info.y + info.h - 1][x].typeId = TileType::eWall;
	}
	// 左右の壁を設定
	for (int y = info.y + 1; y < info.y + info.h - 1; y++)
	{
		mMapData[y][info.x].typeId = TileType::eWall;
		mMapData[y][info.x + info.w - 1].typeId = TileType::eWall;
	}
}

// 四隅の柱を設定
void CDangeonMap::CreateRoomPillar(const RoomInfo& info)
{
	mMapData[info.y][info.x].typeId = TileType::ePillar;	// 左上
	mMapData[info.y][info.x + info.w - 1].typeId = TileType::ePillar;  // 右上
	mMapData[info.y + info.h - 1][info.x].typeId = TileType::ePillar;  // 左下
	mMapData[info.y + info.h - 1][info.x + info.w - 1].typeId = TileType::ePillar;	 // 右下
}

// 扉の設定
void CDangeonMap::CreateRoomDoor(const RoomInfo& info)
{
}

#if _DEBUG
// 区画のタイルのデバック表示
void CDangeonMap::PrintSection()
{
	for (int y = 0; y < ROOM_HEIGHT; y++)
	{
		for (int x = 0; x < ROOM_WIDTH; x++)
		{
			if (mMapData[y][x].typeId == TileType::None) printf("0");
			else if (mMapData[y][x].typeId == TileType::eFloor) printf("1");
			else if (mMapData[y][x].typeId == TileType::eWall) printf("2");
			else if (mMapData[y][x].typeId == TileType::ePillar) printf("3");

		}
		printf("\n");
	}
}
#endif