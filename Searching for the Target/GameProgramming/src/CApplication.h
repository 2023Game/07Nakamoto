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
#include "CMoveFloor.h"
#include "CCube.h"


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
	
	//CCube mCube;

	//モデルクラスのインスタンス作成
	CModel mModel;
	//的のモデル
	CModel mModelTarget;
	//マップのモデル
	CModel mModelMap;
	//背景モデル
	CModel mBackGround;

	CModel mRed;

	//モデルからコライダを生成
	CColliderMesh mColliderMesh;
	CColliderMesh mColliderMesh2;
	
	//CColliderMesh mColliderMeshRed;

	CInput mInput;
	CVector mEye;

	//モデルビュー逆行列
	static CMatrix mModelViewInverse;
};