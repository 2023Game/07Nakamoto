#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CInteractObject.h"

class CCollider;

// スイッチのクラス
class CPushSwitch : public CInteractObject
{
public:
	// コンストラクタ
	CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size);
	// デストラクタ
	~CPushSwitch();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;
	
	// スイッチが押されたかどうか
	bool IsOnSwtch();

	// 番号の取得
	//int GetNumber();

	// 調べる
	void Interact() override;

	// 描画処理
	void Render() override;

private:
	CModel* mpModel;	// スイッチのモデルデータ
	CCollider* mpCollider;	// スイッチのコリジョンデータ

	bool mSwitch;	// スイッチがオンかオフか
	int mNumber;	// 番号
	int mNum;		// 何個作られたか

};