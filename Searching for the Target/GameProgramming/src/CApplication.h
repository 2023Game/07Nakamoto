#pragma once
#include "CModel.h"
#include "CInput.h"
#include "CCharacter.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"

class CApplication
{
public:
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();

	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();

private:
	CPlayer mPlayer;	
	
	//モデルクラスのインスタンス作成
	CModel mModel;
	//弾のモデル
	CModel mModelBullet;
	//的のモデル
	CModel mModelTarget;
	//マップのモデル
	CModel mModelMap;
	//背景モデル
	CModel mBackGround;

	//モデルからコライダを生成
	CColliderMesh mColliderMesh;
	CColliderMesh mColliderMesh2;

	CInput mInput;
	CVector mEye;

	//モデルビュー逆行列
	static CMatrix mModelViewInverse;
};