#ifndef CPLAYER_H
#define CPLAYER_H

#include "CCharacter.h"
#include "CInput.h"
#include "CBullet.h"
#include "CModel.h"

#include "CColliderHitManager.h"

#include <vector>

/*
プレイヤークラス
キャラクタクラスを継承
*/

class CPlayer :public CCharacter , public CColliderHitManager{
public:

	//デフォルトコンストラクタ
	CPlayer();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡縮</param>
	CPlayer(const CVector& pos, 
		const CVector& rot, const CVector& scale);

	//更新処理
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="m">自分のコライダ</param>
	/// <param name="o">相手のコライダ</param>
	void Collision(CCollider* m, CCollider* o);

	//衝突処理
	void Collision();

	//コライダの取得
	CCollider *GetCollider();

	//カーソルのX座標を取得
	float GetFx();
	//カーソルのY座標を取得
	float GetFy();
	//プレイヤーのモデルを取得する
	static CModel* GetModelPlayer();

private:
	static CModel mModelPlayer;

	int mCursorX, mCursorY;	//マウスカーソル取得用
	float mFx, mFy;			//カメラの移動量

	CInput mInput;

	CCollider mSphere;	//球コライダー

	bool mBulletFlag;	//弾が撃たれてるか

	//重力
	//static CVector* mGravity;

	//可変長配列のインスタンス
	CColliderHitManager mCollisionManager;

};

#endif // !CPLAYER_H
