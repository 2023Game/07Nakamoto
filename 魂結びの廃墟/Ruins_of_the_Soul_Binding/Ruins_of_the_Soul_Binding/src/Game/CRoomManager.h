#pragma once
#include "CTask.h"
#include "CRoom.h"

// 部屋を管理するクラス
class CRoomManager : public CTask
{
public:
	// インスタンスを取得
	static CRoomManager* Instance();

	// 部屋をリストに追加
	void Add(CRoom* room);
	// 部屋をリストから取り除く
	void Remove(CRoom* room);

	// 指定したオブジェクトが入っている部屋を返す
	CRoom* GetCurrentRoom(CObjectBase* obj) const;

#if _DEBUG
	// 部屋を表示するかどうか
	bool IsShowRoom() const;
	// 描画
	void Render() override;
#endif


private:
	// コンストラクタ
	CRoomManager();
	// デストラクタ
	~CRoomManager();

	// CRoomManagerのインスタンス
	static CRoomManager* spInstance;
	std::vector<CRoom*> mRooms;	// 部屋のリスト

#if _DEBUG
	bool mIsShowRoom;			// 部屋を表示するかどうか
#endif
};