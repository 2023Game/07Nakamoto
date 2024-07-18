#ifndef CCOLLIDERHITMANAGER_H
#define CCOLLIDERHITMANAGER_H

#include "CCollider.h"

class CColliderHitManager
{
public:
	//処理するコライダ
	enum class ECollider{
		EFLOOR,		//床
		ESLOPE,		//坂
	};

	//デフォルトコンストラクタ
	CColliderHitManager();

	//インスタンスの取得
	static CColliderHitManager* GetInstance();

	/// <summary>
	/// 可変長配列に追加する
	/// </summary>
	/// <param name="o">プレイヤーと当たったコライダ</param>
	void AddColliders(CCollider* o);

	//タグの確認
	CCollider *HitColliderSerch();

	//衝突判定
	void HitCollision(CCollider* m, CCollider* o);

	//修正値の取得
	CVector GetAjustRote();

	//処理するコライダの種類の取得
	ECollider GetCol();

	//可変長配列を削除
	void Delete();

private:
	//可変長配列にプレイヤーと当たったコライダを格納する
	std::vector<CCollider*> static mpHitColliders;

	static CColliderHitManager* mpInstance;

	//回転値の修正値を格納
	CVector ajustRote;

	ECollider mCol;
};

#endif // !CCOLLIDERHITMANAGER_H

