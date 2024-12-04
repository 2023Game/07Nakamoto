#include "CPushSwitch.h"
#include "CPushSwitchManager.h"
#include "Primitive.h"

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
	CModel* colModel = CResourceManager::Get<CModel>("Switch");
	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);

	// 位置と向きとサイズを設定
	Position(pos);
	Rotation(angle);
	Scale(size);

	// 管理クラスのリストの数を番号にしようかな
	mNumber = mNum;
}

// デストラクタ
CPushSwitch::~CPushSwitch()
{
	// コライダーを削除
	SAFE_DELETE(mpColliderMesh);
}

// TODO：プレイヤーがスイッチを押せる範囲に入っているかどうか
//bool CPushSwitch::IsRangePlayer() const
//{
//	return false;
//}

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
