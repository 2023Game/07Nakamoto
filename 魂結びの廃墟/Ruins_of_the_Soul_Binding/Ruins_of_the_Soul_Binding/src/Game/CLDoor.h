#pragma once
#include "CInteractObject.h"
#include "CColliderMesh.h"

class CLDoor : public CInteractObject
{
public:
	// コンストラクタ
	CLDoor(const CVector& pos, const CVector& angle, const CVector& openPos);
	// デストラクタ
	~CLDoor();

	// 調べる
	void Interact() override;

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	CModel* mpL_Door;	// モデルデータ
	CModel* mpL_DoorCol;// コライダーデータ

	CColliderMesh* mpL_DoorColliderMesh;	// コライダー

	bool mIsOpened;	// 扉が開いているかどうか
	bool mIsBroken;	// 扉が壊れたどうか

	CVector mClosePos;	// 扉が閉まっている時の座標
	CVector mOpenPos;	// 扉が開いている時の座標
	float mAnimTime;	// 開閉時間
	float mElapsedTime;	// 経過時間保存用
	bool mIsPlaying;	// 開閉中かどうか
};