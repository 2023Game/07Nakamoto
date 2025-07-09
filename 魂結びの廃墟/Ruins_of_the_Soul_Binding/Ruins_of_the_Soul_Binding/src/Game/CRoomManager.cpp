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

}

// デストラクタ
CRoomManager::~CRoomManager()
{
}

// インスタンスを取得
CRoomManager* CRoomManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CRoomManager();
	}

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

// 後更新
void CRoomManager::LateUpdate()
{
	auto players = CPlayerManager::Instance()->GetPlayers();
	for (CPlayerBase* player : players)
	{
		const CBounds& playerBounds = player->GetBounds();
		CRoom* inRoom = nullptr;
		for (CRoom* room : mRooms)
		{
			const CBounds& roomBounds = room->GetBounds();
			// 部屋とプレイヤーmのバウンディングボックスが触れていたら
			if (CBounds::Intersect(roomBounds, playerBounds))
			{
				// 入っている部屋を設定
				inRoom = room;
				break;
			}
		}

		player->SetRoom(inRoom);
	}
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
