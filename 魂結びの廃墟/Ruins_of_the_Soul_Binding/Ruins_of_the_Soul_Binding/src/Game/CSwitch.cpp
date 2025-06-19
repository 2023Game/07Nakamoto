#include "CSwitch.h"
#include "CColliderMesh.h"
#include "CStand.h"

#define PRESSED_OFFSET_POS 0.8f	// 押されているときの座標

// コンストラクタ
CSwitch::CSwitch(const CVector& pos)
	: CObjectBase(ETag::eField)
	, mSwitch(false)
	, mDefaultPos(pos)
	, mOffSetPos(mDefaultPos)
{
	mpButtonModel = CResourceManager::Get <CModel>("Button");
	mpButtonColMesh = new CColliderMesh(this, ELayer::eObject, mpButtonModel, true);

	mpButtonColMesh->SetCollisionTags({ ETag::ePlayer, ETag::eEnemy });
	mpButtonColMesh->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eEnemy ,
		}
	);

	Position(mDefaultPos);
	mOffSetPos.Y(mDefaultPos.Y() - PRESSED_OFFSET_POS);

	// スイッチの台を生成
	new CStand(pos);
}

// デストラクタ
CSwitch::~CSwitch()
{
	if (mpButtonColMesh != nullptr)
	{
		SAFE_DELETE(mpButtonColMesh);
	}
}

// ボタンが押されているかどうか
bool CSwitch::IsSwitchOn()
{
	return mSwitch;
}

void CSwitch::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::ePlayer)
	{
		if (self == mpButtonColMesh)
		{
			mSwitch = true;
		}
	}
}

// 更新
void CSwitch::Update()
{
	if (!mSwitch)
	{
		Position(mDefaultPos);
	}
	else
	{
		Position(mOffSetPos);
	}

	mSwitch = false;
}

// 描画
void CSwitch::Render()
{
	mpButtonModel->Render(Matrix());
}
