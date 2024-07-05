#pragma once
#include "CModelX.h"

#include "CTexture.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include <vector>
#include "CColliderTriangle.h"

#include "CSound.h"
#include "CModel.h"
#include "CCharacter3.h"
#include "CTaskManager.h"
#include "CColliderMesh.h"
#include "CXCharacter.h"
#include "CXPlayer.h"
#include "CXEnemy.h"

class CApplication
{
private:
	//キャラクタのインスタンス
	CXCharacter mCharacter;
	//キャラクタのインスタンス
	CXPlayer mXPlayer;

	CMatrix mMatrix;

	//モデルからコライダを生成
	CColliderMesh mColliderMesh;

	//三角コライダの作成
	//削除 CColliderTriangle mColliderTriangle;
	//削除 CColliderTriangle mColliderTriangle2;
	
	//C5モデル
	CModel mModelC5;
	//CPlayer mPlayer;
	//CCharacter3 mCharacter;
	CModel mBackGround;	//背景モデル
	//モデルクラスのインスタンス作成
	CModel mModel;

	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー

	//モデルビューの逆行列
	static CMatrix mModelViewInverse;

	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	EState mState;
//	CCharacter mRectangle;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	CEnemy* mpEnemy;
//	CBullet* mpBullet;
	CInput mInput;
	CFont mFont;
	//CCharacterのポインタの可変長配列
//	std::vector<CCharacter*> mCharacters;

public:
	CModelX mModelX;

	CXEnemy mXEnemy;

	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();

	static CTexture* Texture();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};