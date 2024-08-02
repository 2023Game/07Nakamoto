#ifndef CSWITCH_H
#define CSWITCH_H

#include "CCharacter.h"
#include "CCollider.h"
#include "CModel.h"

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
	CSwitch(CModel* model, const CVector& pos,
		const CVector& rot, const CVector& scale);

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

	//スイッチのモデルを取得
	static CModel* GetModelSwitch();
private:
	static CModel mModelSwitch;

	//スイッチのオンオフ
	bool mFlag;

	//球コライダ
	CCollider mCollider;
};


#endif // !CSWITCH_H

