#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include  <functional>

class CSwitch;

class CSwitchDoor : public CObjectBase
{
public:
	//コンストラクタ
	CSwitchDoor(const CVector& pos, const CVector& angle, const CVector& openPos,
		std::string modelName, std::string colName);
	// デストラクタ
	~CSwitchDoor();

	// 接続するスイッチを追加
	void AddSwitch(CSwitch* sw);

	// 持ち主を設定する
	void SetOwner(CObjectBase* owner);
	// 扉の開閉状態が切り替わった時に呼び出す関数のポインタを設定
	void SetOnChangeFunc(std::function<void()> func);

	// ドアが開いているかどうか
	bool IsOpened() const;

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
	// スイッチが全て押されているかどうか
	bool IsAllSwitchOn() const;

	CModel* mpModel;	// モデルデータ
	CColliderMesh* mpModelColliderMesh;	// コライダー

	bool mIsOpened;	// 扉が開いているかどうか

	CVector mClosePos;	// 扉が閉まっている時の座標
	CVector mOpenPos;	// 扉が開いている時の座標
	float mAnimTime;	// 開閉時間
	float mElapsedTime;	// 経過時間保存用
	bool mIsPlaying;	// 開閉中かどうか

	// この扉を開くために押す必要があるスイッチ
	std::list<CSwitch*> mpSwitches;


	CObjectBase* mpOwner;	//	このドアの持ち主
	// ドアの開閉状態が切り替わった時に呼び出す関数のポインタ
	std::function<void()> mOnChangeFunc;


};