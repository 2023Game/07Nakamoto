#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// 壁表示クラス
class CWall : public CObjectBase
{
public:
	// コンストラクタ
	CWall(const CVector& pos, const CVector& angle, const CVector& size);
	// デストラクタ
	~CWall();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	CModel* mpModel;	// 壁のモデルデータ
	CColliderMesh* mpColliderMesh;	// 壁のコリジョンデータ
};