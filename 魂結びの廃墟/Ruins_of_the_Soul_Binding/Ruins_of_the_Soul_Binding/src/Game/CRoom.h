#pragma once
#include <string>

struct RoomData
{
	CVector center;
	CVector size;
};

// 部屋クラス
class CRoom
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="center">部屋の中心位置</param>
	/// <param name="size">部屋のサイズ</param>
	/// <param name="name">部屋の名前</param>
	CRoom(std::vector<RoomData> roomData, std::string name);
	// デストラクタ
	~CRoom();

	// 部屋の名前を取得
	std::string GetName() const;
	// 部屋のバウンディングボックスを返す
	const std::vector<CBounds>& GetBounds() const;

#if _DEBUG
	// 描画
	void Render();
#endif

private:
	std::vector<RoomData> mRoomData;
	std::string mName;	// 部屋の名前
	std::vector<CBounds> mBounds;	// 部屋のバウンディングボックス
};