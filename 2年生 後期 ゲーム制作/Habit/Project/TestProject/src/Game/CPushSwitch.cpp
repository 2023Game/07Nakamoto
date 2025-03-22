#include "CPushSwitch.h"
#include "Primitive.h"
#include "CPlayer2.h"
#include "Maths.h"
#include "CColliderSphere.h"
#include "CBillBoard.h"
#include "CInteractUI.h"

#define POSITION CVector(0.0f,15.0f,0.0f)

// コンストラクタ
CPushSwitch::CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size)
	: mSwitch(false)
{
	// スイッチのモデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Switch");

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

	mInteractStr = "オンにする";

	// インタラクトボタンの表示
	mpInteractUI = new CInteractUI(this);
	mpInteractUI->Position(Position() + POSITION);

}

// デストラクタ
CPushSwitch::~CPushSwitch()
{
	// コライダーを削除
	SAFE_DELETE(mpCollider);
	SAFE_DELETE(mpInteractUI);
}

// スイッチの状態がオンかオフか
bool CPushSwitch::IsOnSwtch()
{
	return mSwitch;
}

// 調べる
void CPushSwitch::Interact()
{
	mSwitch = !mSwitch;
	mInteractStr = mSwitch ? "オフにする" : "オンにする";
}

// 描画処理
void CPushSwitch::Render()
{
	if (mSwitch)
	{
		// スイッチのモデルデータ取得
		mpModel = CResourceManager::Get<CModel>("Switch_ON");
	}
	else
	{
		// スイッチのモデルデータ取得
		mpModel = CResourceManager::Get<CModel>("Switch_OFF");
	}

	mpInteractUI->Render();
	mpModel->Render(Matrix());
}
