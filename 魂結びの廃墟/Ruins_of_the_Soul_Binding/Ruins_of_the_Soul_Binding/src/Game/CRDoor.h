#pragma once
#include "CInteractObject.h"
#include "CColliderMesh.h"

class CRDoor : public CInteractObject
{
public:
	// コンストラクタ
	CRDoor(const CVector& pos, const CVector& angle, const CVector& openPos);
	// デストラクタ
	~CRDoor();

	// 調べる
	void Interact() override;

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	CModel* mpR_Door;	// モデルデータ
	CModel* mpR_DoorCol;// コライダーデータ

	CColliderMesh* mpR_DoorColliderMesh;	// コライダー

	bool mIsOpened;	// 扉が開いているかどうか

	CVector mClosePos;	// 扉が閉まっている時の座標
	CVector mOpenPos;	// 扉が開いている時の座標
	float mAnimTime;	// 開閉時間
	float mElapsedTime;	// 経過時間保存用
	bool mIsPlaying;	// 開閉中かどうか

};