#ifndef CBULLET_H
#define CBULLET_H

#include "CCharacter.h"
#include "CTriangle.h"
#include "CCollider.h"
#include "CModel.h"

/*
弾クラス
三角形を飛ばす
*/
class CBullet : public CCharacter {
public:
	//デフォルトコンストラクタ
	CBullet();

	/// <summary>
	/// 幅と奥行の設定
	/// </summary>
	/// <param name="w">幅</param>
	/// <param name="d">奥行</param>
	void SetSize(float w, float d);

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="m">コライダ1</param>
	/// <param name="o">コライダ2</param>
	void Collision(CCollider* m, CCollider* o);
	
	//更新
	void Update();
	//三角形の弾を描画
	//void Render();

	//衝突処理
	void Collision();

	static CModel* GetModelBullet();
private:
	static CModel mModelBullet;

	//三角形
	CTriangle mT;
	//生存時間
	int mLife;
	//コライダ
	CCollider mCollider;
};

#endif // !CBULLET_H
