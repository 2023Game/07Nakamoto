#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// 扉クラス
class CDoor : public CObjectBase
{
public:
	// コンストラクタ
	CDoor(CObjectBase* owner, const CVector& pos, const CVector& angle, const CVector& size);
	// デストラクタ
	~CDoor();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	CModel* mpModel;	// 扉のモデルデータ
	CColliderMesh* mpColliderMesh;	// 扉のコリジョンデータ

};