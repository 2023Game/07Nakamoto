#include "CRoom.h"
#include "CRoomManager.h"
#include "Primitive.h"

// バウンディングボックスのサイズを部屋のサイズから小さくする長さ
#define BOUNDS_REDUCE_DIST 3.0f

// コンストラクタ
CRoom::CRoom(std::vector<RoomData> roomData, std::string name)
	: mRoomData(roomData)
	, mName(name)
{
	// 部屋管理クラスのリストに自信を登録
	CRoomManager::Instance()->Add(this);

	for (const RoomData& data : mRoomData)
	{
		// 部屋のバウンディングボックスの設定
		CVector boundsSize = data.size;;
		boundsSize.X(boundsSize.X() - BOUNDS_REDUCE_DIST * 2.0f);
		boundsSize.Z(boundsSize.Z() - BOUNDS_REDUCE_DIST * 2.0f);
		CBounds bounds;
		bounds.SetPos(data.center, boundsSize);
		mBounds.push_back(bounds);
	}
}

// デストラクタ
CRoom::~CRoom()
{
	// 部屋管理クラスのリストから自身を取り除く
	CRoomManager::Instance()->Remove(this);
}

// 部屋の名前を取得
std::string CRoom::GetName() const
{
	return mName;
}

// 部屋のバウンディングボックスを返す
const std::vector<CBounds>& CRoom::GetBounds() const
{
	return mBounds;
}

// 描画
void CRoom::Render()
{
	int count = mRoomData.size();
	for (int i = 0; i < count; i++)
	{
		const RoomData& data = mRoomData[i];

		CColor color = CColor::cyan;
		// 部屋のサイズでワイヤーボックスを描画
		Primitive::DrawWireBox(data.center, data.size, color);
		// 部屋のバウンディングボックスのサイズを描画
		color.A(0.5f);
		Primitive::DrawBox(mBounds[i].Center(), mBounds[i].Size(), color);
	}
}
