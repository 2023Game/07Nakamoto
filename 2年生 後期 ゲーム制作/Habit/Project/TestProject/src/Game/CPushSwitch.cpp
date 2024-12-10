#include "CPushSwitch.h"
//#include "CPushSwitchManager.h"
#include "Primitive.h"
#include "CPlayer2.h"
#include "Maths.h"
#include "CColliderSphere.h"

// コンストラクタ
CPushSwitch::CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size)
	: mSwitch(false)
	, mNum(1)
{
	// 管理クラスのリストに自身を追加
	//CPushSwitchManager* pushMgr = CPushSwitchManager::Instance();
	//if (pushMgr != nullptr)
	//{
	//	pushMgr->AddSwitch(this);
	//}

	// スイッチのモデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Switch");

	// スイッチのコライダーを取得
	//CModel* colModel = CResourceManager::Get<CModel>("SwitchCol");
	// スイッチのコライダー生成
	mpCollider = new CColliderSphere
	(
		this, ELayer::eInteractObj,
		4.0f, true
	);
	mpCollider->Position(0.0f, 2.0f, 0.0f);
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer, ELayer::eInteractSearch });

	// 位置と向きとサイズを設定
	Position(pos);
	Rotation(angle);
	Scale(size);

	//mNumber = mNum;

	mInteractStr = "オンにする";
}

// デストラクタ
CPushSwitch::~CPushSwitch()
{
	// コライダーを削除
	SAFE_DELETE(mpCollider);
}


void CPushSwitch::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// スイッチの状態がオンかオフか
bool CPushSwitch::IsOnSwtch()
{
	return mSwitch;
}

// 番号を取得
//int CPushSwitch::GetNumber()
//{
//	return mNumber;
//}

// 調べる
void CPushSwitch::Interact()
{
	mSwitch = !mSwitch;
	mInteractStr = mSwitch ? "オンにする" : "オフにする";
}

// 描画処理
void CPushSwitch::Render()
{
	mpModel->Render(Matrix());
}
