#include "CSwitchFloor.h"
#include "CModel.h"
#include "CColliderMesh.h"
#include "CField.h"

#define ON_OFFSET_POS 0.4f

// コンストラクタ
CSwitchFloor::CSwitchFloor(const CVector& pos)
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpCollider(nullptr)
	, mSwitch(false)
	, mpPushedObject(nullptr)
	, mpLastPushedObject(nullptr)
{
	mpModel = CResourceManager::Get<CModel>("Switch_Floor");

	mpCollider = new CColliderMesh(this, ELayer::eSwitchFloor, mpModel, false);
	mpCollider->SetCollisionTags({ ETag::ePlayer, ETag::eField});
	mpCollider->SetCollisionLayers({ ELayer::ePlayer,ELayer::eMoveCrate });

	Position(pos);
}

// デストラクタ
CSwitchFloor::~CSwitchFloor()
{
	SAFE_DELETE(mpCollider);
}

// ボタンのオンオフを切り替える
void CSwitchFloor::ChangeSwith()
{
	mSwitch = !mSwitch;
}

// 衝突判定
void CSwitchFloor::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpCollider)
	{
		if (other->Layer() == ELayer::ePlayer || other->Layer() == ELayer::eEnemy || other->Layer() == ELayer::eMoveCrate)
		{
			mpPushedObject = other->Owner();

			// 前回のフレームでも触れていたオブジェクトであれば、処理しない
			if (mpPushedObject == mpLastPushedObject)	return;

			mpLastPushedObject = mpPushedObject;
			// スイッチの状態を切り替える
			mSwitch = true;
		}
	}
}

// 更新処理
void CSwitchFloor::Update()
{
	// 前回のフレームでスイッチを踏んでいるオブジェクトが存在しなかった場合
	if (mpPushedObject == nullptr)
	{
		// 前回にスイッチを踏んでいるオブジェクトのポインタを初期化
		mpLastPushedObject = nullptr;
		mSwitch = false;
	}
	// スイッチを踏んでいるオブジェクトのポインタを空にする
	mpPushedObject = nullptr;
}

// 描画処理
void CSwitchFloor::Render()
{
	CMatrix m = Matrix();

	// 感圧板が押されていたら
	if (mSwitch)
	{
		CMatrix transMtx;
		transMtx.Translate(0.0f, -ON_OFFSET_POS, 0.0f);
		m = m * transMtx;
	}
	mpModel->Render(m);
}
