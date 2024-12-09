#include "CPushSwitch.h"
#include "CPushSwitchManager.h"
#include "Primitive.h"
#include "CPlayer2.h"
#include "Maths.h"

// コンストラクタ
CPushSwitch::CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size)
	: mSwitch(false)
	, mNum(1)
{
	// 管理クラスのリストに自身を追加
	CPushSwitchManager* pushMgr = CPushSwitchManager::Instance();
	if (pushMgr != nullptr)
	{
		pushMgr->AddSwitch(this);
	}

	// スイッチのモデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Switch");

	// スイッチのコライダーを取得
	CModel* colModel = CResourceManager::Get<CModel>("SwitchCol");
	// スイッチのコライダー生成
	mpColliderMesh = new CColliderMesh(this, ELayer::eSwitch, colModel, true);

	// 位置と向きとサイズを設定
	Position(pos);
	Rotation(angle);
	Scale(size);

	mNumber = mNum;
}

// デストラクタ
CPushSwitch::~CPushSwitch()
{
	// コライダーを削除
	SAFE_DELETE(mpColliderMesh);
}


// スイッチの状態がオンかオフか
bool CPushSwitch::IsOnSwtch()
{
	return mSwitch;
}

// 番号を取得
int CPushSwitch::GetNumber()
{
	return mNumber;
}

// 描画処理
void CPushSwitch::Render()
{
	mpModel->Render(Matrix());
}
