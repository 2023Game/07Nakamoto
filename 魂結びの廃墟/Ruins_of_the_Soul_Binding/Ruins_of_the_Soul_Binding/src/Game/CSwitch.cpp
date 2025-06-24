#include "CSwitch.h"
#include "CColliderMesh.h"
#include "CStand.h"

#define PRESSED_OFFSET_POS 0.8f	// 押されているときのY座標

// コンストラクタ
CSwitch::CSwitch(const CVector& pos)
	: CObjectBase(ETag::eField)
	, mpPushedObject(nullptr)
	, mSwitch(false)
	, mDefaultPos(pos)
	, mOffSetPos(mDefaultPos)
	, mElapsedTime(0.0f)
{
	mpButtonModel = CResourceManager::Get <CModel>("Button");
	mpButtonColMesh = new CColliderMesh(this, ELayer::eSwitch, mpButtonModel, true);

	mpButtonColMesh->SetCollisionTags({ ETag::eCat,});
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
CSwitch::~CSwitch()
{
	SAFE_DELETE(mpButtonColMesh);
}

// ボタンが押されているかどうか
bool CSwitch::IsSwitchOn()
{
	return mSwitch;
}

// 衝突処理
void CSwitch::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eCat)
	{
		if (self == mpButtonColMesh)
		{
			// 乗っているオブジェクトを設定
			mpPushedObject = other->Owner();
			mElapsedTime = 0.0f;	// 経過時間をリセット
			mSwitch = true;			// スイッチをオン
		}
	}
}

// 更新
void CSwitch::Update()
{
	// スイッチの上に乗っているオブジェクトが存在しなくなった
	if (mpPushedObject == nullptr)
	{
		// 一定時間後にスイッチをオフにする
		if (mElapsedTime < 0.25f)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			mSwitch = false;
		}
	}
	// スイッチを踏んでいるオブジェクトのポインタを空にする
	mpPushedObject = nullptr;

	CDebugPrint::Print("スイッチ：%s\n", mSwitch ? "オン" : "オフ");
}

// 描画
void CSwitch::Render()
{
	CMatrix m = Matrix();
	// スイッチが押されているときだけ、押し込んだ位置に移動
	// （モデルデータの座標だけズラす）
	if (mSwitch)
	{
		CMatrix transMtx;
		transMtx.Translate(0.0f, -PRESSED_OFFSET_POS, 0.0f);
		m = m * transMtx;
	}

	mpButtonModel->Render(m);
}
