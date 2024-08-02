#ifndef CGAME_H
#define CGAME_H

#include "CPlayer.h"
#include "CSlope.h"
#include "CSwitch.h"
#include "CMoveWall.h"
#include "CMoveFloor.h"
#include "CFloor.h"
#include "CObject.h"
#include "CUi.h"

class CGame
{
public:
	//デフォルトコンストラクタ
	CGame();
	//デストラクタ
	~CGame();
	//スタート処理
	void Start();
	//更新処理
	void Update();
	//ゲームクリア処理
	void Clear();
	//ゲームクリア判定
	bool IsClear();
	//ゲームオーバー処理
	void Over();
	//ゲームオーバー判定
	bool IsOver();

	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();
	
private:
	//UIクラスのポインタ
	CUi* mpUi;

	//プレイヤー
	CPlayer* mpPlayer;
	//坂
	//CSlope mSlope;
	//CSlope mSlope2;
	//スイッチ
	//CSwitch mSwhith;
	//動く壁
	//CMoveWall mMoveWall;
	//動く床
	//CMoveFloor mMoveFloor;

	//モデルからコライダを生成
	//坂のコライダ
	//CColliderMesh mCSlope;
	//CColliderMesh mCSlope2;
	//床のコライダ
	//CColliderMesh* mCFloor;
	//オブジェクトのコライダ
	//CColliderMesh* mCObject;

	//モデルビュー逆行列
	static CMatrix mModelViewInverse;
};

#endif // !CGAME_H
