#include "CPressAndHoldSwitch.h"
#include "CColliderMesh.h"
#include "CStand.h"

#define PRESSED_OFFSET_POS 0.8f	// 押されているときのY座標

// コンストラクタ
CPressAndHoldSwitch::CPressAndHoldSwitch(const CVector& pos)
	: CObjectBase(ETag::eField)
	, mpLastPushedObject(nullptr)
	, mpPushedObject(nullptr)
	, mSwitch(false)
	, mDefaultPos(pos)
	, mOffSetPos(mDefaultPos)
	, mElapsedTime(0.0f)
{
	mpButtonModel = CResourceManager::Get <CModel>("Button");
	mpButtonColMesh = new CColliderMesh(this, ELayer::eSwitch, mpButtonModel, true);

	mpButtonColMesh->SetCollisionTags({ ETag::eCat, });
	mpButtonColMesh->SetCollisionLayers
	(
		{
			ELayer::eCat,
		}
	);

	Position(mDefaultPos);
	mOffSetPos.Y(mDefaultPos.Y() - PRESSED_OFFSET_POS);

	// スイッチの台を生成
	new CStand(pos);

}

// デストラクタ
CPressAndHoldSwitch::~CPressAndHoldSwitch()
{
	SAFE_DELETE(mpButtonColMesh);
}

// ボタンが押されているかどうか
bool CPressAndHoldSwitch::IsSwitchOn()
{
	return mSwitch;
}

// ボタンのオンオフ切り替え
void CPressAndHoldSwitch::ChangeSwith()
{
	mSwitch = !mSwitch;
}

// 衝突処理
bool CPressAndHoldSwitch::IsPushedObject(CObjectBase* obj) const
{
}

// 衝突処理
void CPressAndHoldSwitch::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eCat)
	{
		if (self == mpButtonColMesh)
		{
			mpPushedObject = other->Owner();
			// 前回のへれーむでも触れていたオブジェクトであれば、処理しない
			if (mpPushedObject == mpLastPushedObject)	return;

			mpLastPushedObject = mpPushedObject;
			// スイッチの状態を切り替える
			ChangeSwith();

			mElapsedTime = 0.0f;
		}
	}
}

// 更新
void CPressAndHoldSwitch::Update()
{
	// 前回のフレームでスイッチを踏んでいるオブジェクトが存在しなかった場合
	if (mpPushedObject == nullptr)
	{
		if (mElapsedTime < 1.0f)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// 前回にスイッチを踏んでいるオブジェクトのポインタを初期化
			mpLastPushedObject = nullptr;
		}
	}

	if (!mSwitch)
	{
		Position(mDefaultPos);
	}
	else
	{
		Position(mOffSetPos);
	}

	// スイッチを踏んでいるオブジェクトのポインタを空にする
	mpPushedObject = nullptr;
}

// 描画
void CPressAndHoldSwitch::Render()
{
	mpButtonModel->Render(Matrix());
}
