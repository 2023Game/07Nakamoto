#pragma once
#include "CInteractObject.h"
#include "CColliderMesh.h"
#include <functional>

class CDoorBase : public CInteractObject
{
public:
	// コンストラクタ
	CDoorBase(const CVector& pos, const CVector& angle, std::string modelName, std::string colName);
	// デストラクタ
	virtual ~CDoorBase();

	// 持ち主を設定する
	void SetOwner(CObjectBase* owner);
	// ドアの開閉状態が切り替わった時に呼び出す関数のポインタを設定
	void SetOnChangeFunc(std::function<void()> func);

	// 調べる内容のテキスト画像のパスを返す
	std::string GetInteractTextPath() const override;

	// ドアが開いているかどうか
	bool IsOpened() const;

	// 調べられる状態かどうか
	bool CanInteract() const override;
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

	// 描画処理
	void Render() override;

protected:
	bool mIsOpened;	// 扉が開いているかどうか
	bool mIsPlaying;	// 開閉中かどうか

private:
	CModel* mpDoor;	// モデルデータ
	CModel* mpDoorCol;// コライダーデータ

	CColliderMesh* mpDoorColliderMesh;	// コライダー


	CObjectBase* mpOwner;	//	このドアの持ち主
	// ドアの開閉状態が切り替わった時に呼び出す関数のポインタ
	std::function<void()> mOnChangeFunc;

};