#include "CPushSwitch.h"
#include "Primitive.h"
#include "CPlayer2.h"
#include "Maths.h"
#include "CColliderSphere.h"
#include "CBillBoard.h"

#define INTERACT_TEXT_PATH_ON "UI\\Interact\\on.png"
#define INTERACT_TEXT_PATH_OFF "UI\\Interact\\off.png"
#define POSITION CVector(0.0f,15.0f,0.0f)

// コンストラクタ
CPushSwitch::CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size)
	: mSwitch(false)
{
	// 調べるテキストの画像を読み込む
	CResourceManager::Load<CTexture>(INTERACT_TEXT_PATH_ON, INTERACT_TEXT_PATH_ON);
	CResourceManager::Load<CTexture>(INTERACT_TEXT_PATH_OFF, INTERACT_TEXT_PATH_OFF);

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
}

// デストラクタ
CPushSwitch::~CPushSwitch()
{
	// コライダーを削除
	SAFE_DELETE(mpCollider);
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

// 調べる内容のテキスト画像のパスを返す
std::string CPushSwitch::GetInteractTextPath() const
{
	return mSwitch ? INTERACT_TEXT_PATH_OFF : INTERACT_TEXT_PATH_ON;
}

// 調べるUIを表示する座標を返す
CVector CPushSwitch::GetInteractUIPos() const
{
	return Position() + CVector(0.0f, 10.0f, 0.0f);
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

	mpModel->Render(Matrix());
}
