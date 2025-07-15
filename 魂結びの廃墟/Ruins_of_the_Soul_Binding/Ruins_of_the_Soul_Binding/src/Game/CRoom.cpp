#include "CRoom.h"
#include "CRoomManager.h"
#include "Primitive.h"

// バウンディングボックスのサイズを部屋のサイズから小さくする長さ
#define BOUNDS_REDUCE_DIST 3.0f

// コンストラクタ
CRoom::CRoom(const CVector& center, const CVector& size, std::string name)
	: mCenter(center)
	, mSize(size)
	, mName(name)
{
	// 部屋管理クラスのリストに自信を登録
	CRoomManager::Instance()->Add(this);

	// 部屋のバウンディングボックスの設定
	CVector boundsSize = mSize;
	boundsSize.X(boundsSize.X() - BOUNDS_REDUCE_DIST * 2.0f);
	boundsSize.Z(boundsSize.Z() - BOUNDS_REDUCE_DIST * 2.0f);
	mBounds.SetPos(mCenter, boundsSize);
}

// デストラクタ
CRoom::~CRoom()
{
	// 部屋管理クラスのリストから自身を取り除く
	CRoomManager::Instance()->Remove(this);
}

// 部屋の中心位置を取得
const CVector& CRoom::GetCenter() const
{
	return mCenter;
}

// 部屋のサイズを取得
const CVector& CRoom::GetSize() const
{
	return mSize;
}

// 部屋の名前を取得
std::string CRoom::GetName() const
{
	return mName;
}

// 部屋のバウンディングボックスを返す
const CBounds& CRoom::GetBounds() const
{
	return mBounds;
}

// 描画
void CRoom::Render()
{
	CColor color = CColor::cyan;
	// 部屋のサイズでワイヤーボックスを描画
	Primitive::DrawWireBox(mCenter, mSize, color);
	// 部屋のバウンディングボックスのサイズを描画
	color.A(0.5f);
	Primitive::DrawBox(mBounds.Center(), mBounds.Size(), color);
}
