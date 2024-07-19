#ifndef CPLAYER_H
#define CPLAYER_H

#include "CCharacter.h"
#include "CInput.h"
#include "CBullet.h"
#include "CColliderLine.h"
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

	/// <summary>
	/// プレイヤーに当たっていっるコライダを格納
	/// </summary>
	/// <param name="o">相手のコライダ</param>
	//void AddHitCollider(CCollider* o);

	//タグの確認
	//CCollider HitColliderSerch();
	//可変長配列の削除
	//void Delete();

	//球コライダの取得
	//CCollider GetCollider();

private:
	int mCursorX, mCursorY;	//マウスカーソル取得用
	float mFx, mFy;			//カメラの移動量

	CInput mInput;

	//bool mSlopeFlag;

	CColliderLine mLine;	//前後の線分コライダ
	CColliderLine mLine2;	//上下の線分コライダ
	CColliderLine mLine3;	//左右の線分コライダ

	//CColliderLine mLine4;	//下の線分コライダ

	CCollider mSphere;		//球コライダー

	bool mBulletFlag;

	//可変長配列のインスタンス
	CColliderHitManager mCollisionManager;

};

#endif // !CPLAYER_H
