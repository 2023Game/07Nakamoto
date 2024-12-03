#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// ボタンのクラス
class CPushButton : public CObjectBase
{
public:
	// インスタンスのポインタの取得
	static CPushButton* Instance();
	// コンストラクタ
	CPushButton(const CVector& pos, const CVector& angle, const CVector& size);
	// デストラクタ
	~CPushButton();

	// プレイヤーがボタンを押せる範囲に入ったかどうか
	bool isRangePlayer() const;

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	// ボタンのインスタンス
	static CPushButton* spInstance;

	CModel* mpModel;	// ボタンのモデルデータ
	CColliderMesh* mpColliderMesh;	// ボタンのコリジョンデータ

	bool mPush;	//ボタンが押されているか
};