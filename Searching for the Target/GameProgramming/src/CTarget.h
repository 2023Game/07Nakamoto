#ifndef CTARGET_H
#define CTARGET_H

#include "CCharacter.h"
#include "CCollider.h"
#include "CModel.h"

/*
ターゲットクラス
キャラクタクラスを継承
*/
class CTarget :public CCharacter {
public:
	//Targetのタイプ
	enum class EState
	{
		ESTAY,		//滞在
		EMOVE1,		//横移動
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">位置</param>
	/// <param name="rotation">回転</param>
	/// <param name="scale">拡縮</param>
	/// <param name="state">状態</param>
	CTarget(CModel* model, const CVector& position,
		const CVector& rotation, const CVector& scale, EState state);

	//デストラクタ
	~CTarget();

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

	//的のモデルを取得する
	static CModel* GetModelTarget();
	//的の数を取得する
	static int GetNum();
	//初期設定
	static void InitNum();

private:
	static CModel mModelTarget;
	//的の数
	static int sNum;

	//状態
	EState mState;
	//コライダ
	CCollider mCollider;
	//移動した距離
	int count;
};

#endif // !CTARGET_H
