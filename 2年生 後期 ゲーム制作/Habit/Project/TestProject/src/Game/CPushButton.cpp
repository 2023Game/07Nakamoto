#include "CPushButton.h"
#include "CColliderTriangle.h"
#include "CPlayer2.h"
#include "Primitive.h"

// プレイヤーのインスタンス
CPushButton* CPushButton::spInstance = nullptr;

// コンストラクタ
CPushButton::CPushButton(const CVector& pos, const CVector& angle, const CVector& size)
	: mPush(false)
{
	// インスタンスの設定
	spInstance = this;

	// ボタンのモデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Wall");

	// ボタンのコライダーを取得
	CModel* colModel = CResourceManager::Get<CModel>("WallCol");
	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);

	// 位置と向きとサイズを設定
	Position(pos);
	Rotation(angle);
	Scale(size);
}

// デストラクタ
CPushButton::~CPushButton()
{
	// コライダーを削除
	SAFE_DELETE(mpColliderMesh);
}

bool CPushButton::isRangePlayer() const
{
	return false;
}

// インスタンスの取得
CPushButton* CPushButton::Instance()
{
	// プレイヤーが存在しない場合
	CPlayer2* player = CPlayer2::Instance();
	if (player == nullptr)return false;

	//プレイヤーの座標を取得
	CVector playerPos = player->Position();
	// 自分自身の座標を取得
	//CVector pos = Position();
	// 自身からプレイヤーまでのベクトルを求める
	//CVector  vec = playerPos - pos;
	//vec.Y(0.0f);	//プレイヤーとの高さの差を考慮しない

	return spInstance;
}

// 更新処理
void CPushButton::Update()
{
}

// 描画処理
void CPushButton::Render()
{
	mpModel->Render(Matrix());
}
