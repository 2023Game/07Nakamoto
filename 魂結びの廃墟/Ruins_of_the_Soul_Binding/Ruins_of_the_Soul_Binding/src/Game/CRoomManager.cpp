#include "CRoomManager.h"
#include "CPlayerManager.h"
#include "CPlayerBase.h"

#if _DEBUG
#include "CDebugInput.h"
#endif

CRoomManager* CRoomManager::spInstance = nullptr;

// コンストラクタ
CRoomManager::CRoomManager()
	: CTask(ETaskPriority::eRoom,0,ETaskPauseType::eGame)
#if _DEBUG
	, mIsShowRoom(false)
#endif
{
	spInstance = this;
}

// デストラクタ
CRoomManager::~CRoomManager()
{
	spInstance = nullptr;

	// 部屋のデータを全て削除
	auto itr = mRooms.begin();
	while (itr != mRooms.end())
	{
		CRoom* room = *itr;
		delete room;
		itr = mRooms.erase(itr);
	}
}

// インスタンスを取得
CRoomManager* CRoomManager::Instance()
{
	return spInstance;
}

// 部屋をリストに追加
void CRoomManager::Add(CRoom* room)
{
	mRooms.push_back(room);
}

// 部屋をリストから取り除く
void CRoomManager::Remove(CRoom* room)
{
	auto result = std::remove(mRooms.begin(), mRooms.end(), room);
	mRooms.erase(result);
}

// 指定したオブジェクトが入っている部屋を返す
CRoom* CRoomManager::GetCurrentRoom(CObjectBase* obj) const
{
	const CBounds& objBounds = obj->GetBounds();
	CRoom* inRoom = nullptr;
	for (CRoom* room : mRooms)
	{
		const std::vector<CBounds>& roomBounds = room->GetBounds();
		for (const CBounds& bounds : roomBounds)
		{
			// 部屋とオブジェクトのバウンディングボックスが触れていたら
			if (CBounds::Intersect(bounds, objBounds))
			{
				// 入っている部屋を返す
				return room;
			}
		}
	}

	return nullptr;
}

#if _DEBUG
// 部屋を表示するかどうか
bool CRoomManager::IsShowRoom() const
{
	return mIsShowRoom;
}

// 描画
void CRoomManager::Render()
{
	// [SHIFT]+[1]キーで部屋の表示フラグを切り替え
	if (CDebugInput::Key(VK_SHIFT) && CDebugInput::PushKey('1'))
	{
		mIsShowRoom = !mIsShowRoom;
	}

	// 表示フラグが立っていなければ、処理しない
	if (!mIsShowRoom)	return;

	// リストに登録してある部屋を描画
	for (CRoom* room : mRooms)
	{
		room->Render();
	}
}
#endif
