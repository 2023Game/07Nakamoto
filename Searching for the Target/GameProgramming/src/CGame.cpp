#include "CGame.h"
#include "CTarget.h"
#include "CApplication.h"
#include "CCollider.h"

//デフォルトコンストラクタ
CGame::CGame()
	: mpUi(nullptr)
{
	//的の数の初期化
	CTarget::InitNum();

	//Uiの生成
	mpUi = new CUi();
	//プレイヤーの生成
	mpPlayer = new CPlayer(CVector(0.0f, 0.0f, -5.0f), CVector(0.0f, 180.0f, 0.0f), 
		CVector(1.5f, 1.5f, 1.5f), CPlayer::GetModelPlayer());

	//的のコライダを生成
	new CTarget(CTarget::GetModelTarget(), CVector(-21.0f, 4.5f, -1.0f),
		CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(CTarget::GetModelTarget(), CVector(85.0f, 24.0f, 35.0f),
		CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(CTarget::GetModelTarget(), CVector(1.5f, 10.0f, 33.0f),
		CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(CTarget::GetModelTarget(), CVector(30.0f, 40.0f, -100.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(CTarget::GetModelTarget(), CVector(60.0f, 9.0f, 5.0f),
		CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::EMOVE1);

	//スイッチの生成
	new CSwitch(CSwitch::GetModelSwitch(),
		CVector(-17.0f, 10.0f, -11.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	//動く床
	new CMoveFloor(CVector(31.0f, 12.0f, -40.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	//坂の生成
	new CSlope(CVector(-16.5f, 5.325f, 33.0f), CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.065f, 0.8f), CSlope::GetModelSlope());
	new CSlope(CVector(31.0f, 4.25f, -21.25f), CVector(0.0f, 90.0f, 0.0f),
		CVector(1.0f, 0.875f, 0.8f), CSlope::GetModelSlope());
	
	//床
	new CFloor(CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CFloor::GetModelFloor());
	//オブジェクト
	new CObject(CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CObject::GetModelObject());

	//視点
	CVector e, c, u;
	//視点を求める
	e = CVector(60.0f, 2.0f, 10.0f);
	//注視点を求める
	c = CVector(0.0f, 0.0f, 0.0f);
	//上方向を求める
	u = CVector(0, 1, 0);
	//カメラ設定
	gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.GetTranspose();
	mModelViewInverse.SetM(0, 3, 0);
	mModelViewInverse.SetM(1, 3, 0);
	mModelViewInverse.SetM(2, 3, 0);

	//タスクマネージャの更新
	CTaskManager::GetInstance()->Update();
	
}

//デストラクタ
CGame::~CGame()
{
	//全てのインスタンス削除
	CTaskManager::GetInstance()->AllDelete();
	CCollisionManager::GetInstance()->AllDelete();
	//Uiを生成しているとき
	if (mpUi != nullptr)
	{
		//UIを削除し、初期化
		delete mpUi;
		mpUi = nullptr;
	}
}

//スタート処理
void CGame::Start()
{
	//視点
	CVector e, c, u;
	//視点を求める
	e = CVector(60.0f, 2.0f, 10.0f);
	//注視点を求める
	c = CVector(0.0f, 0.0f, 0.0f);
	//上方向を求める
	u = CVector(0, 1, 0);
	//カメラ設定
	gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.GetTranspose();
	mModelViewInverse.SetM(0, 3, 0);
	mModelViewInverse.SetM(1, 3, 0);
	mModelViewInverse.SetM(2, 3, 0);

	//マップの描画
	CColliderMesh::GetModelFloor()->Render();
	CColliderMesh::GetModelObject()->Render();

	//タスクマネージャの描画
	CTaskManager::GetInstance()->Render();

	mpUi->State();
}

//更新処理
void CGame::Update() 
{
	//タスクマネージャの更新
	CTaskManager::GetInstance()->Update();
	//コリジョンマネージャの衝突判定
	CTaskManager::GetInstance()->Collision();

	//プレイヤーの衝突処理
	mpPlayer->HitCollision(mpPlayer->GetCollider(), CColliderHitManager::GetInstance()->HitColliderSerch());

	//HitManegerのリストの削除
	CColliderHitManager::GetInstance()->Delete();

	//視点
	CVector e, c, u;
	//視点を求める
	e = mpPlayer->GetPosition() + CVector(0, 5, -8) * mpPlayer->GetMatrixRotate();
	//注視点を求める
	c = mpPlayer->GetPosition() + CVector(0.0f, 3.0f, 0.0f);
	//上方向を求める
	u = CVector(0, 1, 0) * mpPlayer->GetMatrixRotate();
	//カメラ設定
	gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.GetTranspose();
	mModelViewInverse.SetM(0, 3, 0);
	mModelViewInverse.SetM(1, 3, 0);
	mModelViewInverse.SetM(2, 3, 0);

	//マップの描画
	CColliderMesh::GetModelFloor()->Render();
	CColliderMesh::GetModelObject()->Render();

	//タスクリストの削除
	CTaskManager::GetInstance()->Delete();
	//タスクマネージャの描画
	CTaskManager::GetInstance()->Render();
	//コリジョンマネージャの描画(コライダ確認用)
	//CCollisionManager::GetInstance()->Render();
	
	mpUi->Time();
	mpUi->Render();
	
}

//ゲームクリア処理
void CGame::Clear()
{
	mpUi->Clear();
}

//ゲームクリア判定
bool CGame::IsClear()
{
	return CTarget::GetNum() == 0 ;
}

//ゲームオーバー処理
void CGame::Over()
{
	
}

bool CGame::IsOver()
{
	return CUi::GetTime() == 0;
}

CMatrix CGame::mModelViewInverse;
//モデルビュー行列の取得
const CMatrix& CGame::ModelViewInverse()
{
	return mModelViewInverse;
}