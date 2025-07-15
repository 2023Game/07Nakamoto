#pragma once
#include <string>

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
	CRoom(const CVector& center, const CVector& size, std::string name);
	// デストラクタ
	~CRoom();

	// 部屋の中心位置を取得
	const CVector& GetCenter() const;
	// 部屋のサイズを取得
	const CVector& GetSize() const;
	// 部屋の名前を取得
	std::string GetName() const;
	// 部屋のバウンディングボックスを返す
	const CBounds& GetBounds() const;

#if _DEBUG
	// 描画
	void Render();
#endif

private:
	CVector mCenter;	// 部屋の中心位置
	CVector mSize;		// 部屋のサイズ
	std::string mName;	// 部屋の名前
	CBounds mBounds;	// 部屋のバウンディングボックス
};