#pragma once
#include "CInteractObject.h"
#include "CColliderMesh.h"

class CDoor : public CInteractObject
{
public:
	// コンストラクタ
	CDoor(const CVector& pos, const CVector& angle, const CVector& openPos, std::string modelName, std::string colName);
	// デストラクタ
	virtual ~CDoor();

	// 調べる
	void Interact() override;
	// 壊れた時の処理
	void Death();

	/// <summary>
	/// レイとフィールドオブジェクトの衝突判定
	/// </summary>
	/// <param name="start">レイの開始位置</param>
	/// <param name="end">レイの終了位置</param>
	/// <param name="hit">衝突情報返却用</param>
	/// <returns>衝突したら、trueを返す</returns>
	bool CollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit) override;

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	CModel* mpDoor;	// モデルデータ
	CModel* mpDoorCol;// コライダーデータ

	CColliderMesh* mpDoorColliderMesh;	// コライダー

	bool mIsOpened;	// 扉が開いているかどうか

	CVector mClosePos;	// 扉が閉まっている時の座標
	CVector mOpenPos;	// 扉が開いている時の座標
	float mAnimTime;	// 開閉時間
	float mElapsedTime;	// 経過時間保存用
	bool mIsPlaying;	// 開閉中かどうか
};