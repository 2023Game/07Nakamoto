#ifndef CSWITCH_H
#define CSWITCH_H

#include "CCharacter.h"
#include "CCollider.h"

class CSwitch :public CCharacter
{
public:
	//デフォルトコンストラクタ
	CSwitch();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡縮</param>
	void SetSwitch(CModel* model, const CVector& pos,
		const CVector& rot, const CVector& scale);

	void SetSwitch(CSwitch* parent, CModel* model,
		const CVector& spos, const CVector& srot,const CVector& sscale,
		const CVector& dpos, const CVector& drot, const CVector& dscale);

	//フラグを取得
	bool GetFlag();

	//更新処理
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="m">コライダ1</param>
	/// <param name="o">コライダ2</param>
	void Collision(CCollider* m, CCollider* o);

	//衝突処理
	void Collision();

private:
	//スイッチのオンオフ
	bool mFlag;

	//球コライダ
	CCollider mCollider;
};


#endif // !CSWITCH_H

