#ifndef CPLAYER_H
#define CPLAYER_H

#include "CCharacter.h"
#include "CInput.h"
#include "CBullet.h"
#include "CColliderLine.h"
#include "CModel.h"

/*
プレイヤークラス
キャラクタクラスを継承
*/

class CPlayer :public CCharacter {
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

	//衝突処理
	void Collision(CCollider* m, CCollider* o);

	//衝突処理
	void Collision();

	//カーソルのX座標を取得
	float GetFx();
	//カーソルのY座標を取得
	float GetFy();
private:
	int mCursorX, mCursorY;	//マウスカーソル取得用
	float mFx, mFy;			//カメラの移動量

	CInput mInput;

	CColliderLine mLine;	//前後の線分コライダ
	CColliderLine mLine2;	//上下の線分コライダ
	CColliderLine mLine3;	//左右の線分コライダ

	bool mBulletFlag;
};

#endif // !CPLAYER_H
