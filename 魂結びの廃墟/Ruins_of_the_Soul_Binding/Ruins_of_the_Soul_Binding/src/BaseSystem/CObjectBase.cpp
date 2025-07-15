#include "CObjectBase.h"
#include "CCollider.h"
#include "Maths.h"

// コンストラクタ
CObjectBase::CObjectBase(ETag tag,
	ETaskPriority prio, int sortOrder,
	ETaskPauseType pause,
	bool dontDelete, bool addTaskList)
	: CTask(prio, sortOrder, pause, dontDelete, addTaskList)
	, mTag(tag)
	, mIsEnableCol(true)
	, mMaxHp(10)
	, mHp(mMaxHp)
	, mDepth(0.0f)
	, mColor(CColor::white)
	, mpNavNode(nullptr)
	, mpRoom(nullptr)
{
}

// デストラクタ
CObjectBase::~CObjectBase()
{
}

// オブジェクト削除を伝える関数
void CObjectBase::DeleteObject(CObjectBase* obj)
{
}

// オブジェクトタグを取得
ETag CObjectBase::Tag() const
{
	return mTag;
}

// 衝突判定を行うか設定
void CObjectBase::SetEnableCol(bool isEnable)
{
	mIsEnableCol = isEnable;
}

// 衝突判定を行うかどうか
bool CObjectBase::IsEnableCol() const
{
	return mIsEnableCol && IsEnable();
}

// カラーを設定
void CObjectBase::SetColor(const CColor& color)
{
	mColor = color;
}

// カラーを取得
const CColor& CObjectBase::GetColor() const
{
	return mColor;
}

// アルファ値設定
void CObjectBase::SetAlpha(float alpha)
{
	mColor.A(Math::Clamp01(alpha));
}

// アルファ値取得
float CObjectBase::GetAlpha() const
{
	return mColor.A();
}

// カメラまでの距離を計算
void CObjectBase::CalcDepth()
{
	CCamera* camera = CCamera::CurrentCamera();
	if (camera == nullptr) return;

	CVector camPos = camera->Position();
	CVector pos = Position();

	mDepth = (camPos - pos).LengthSqr();
}

// カメラからの距離を取得
float CObjectBase::GetDepth() const
{
	return mDepth;
}

// 自身が入っている部屋を設定
void CObjectBase::SetRoom(CRoom* room)
{
	mpRoom = room;
}

// 自身が入っている部屋のポインタを返す
CRoom* CObjectBase::GetRoom() const
{
	return mpRoom;
}

// 自身のバウンディングボックスを返す
const CBounds& CObjectBase::GetBounds() const
{
	return CBounds();
}

// 衝突処理
void CObjectBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// レイとオブジェクトの衝突判定
bool CObjectBase::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	return false;
}

// 攻撃中か
bool CObjectBase::IsAttacking() const
{
	return false;
}

// 攻撃開始
void CObjectBase::AttackStart()
{
	// 攻撃がヒットしたオブジェクトのリストを初期化
	mAttackHitObjects.clear();
}

// 攻撃終了
void CObjectBase::AttackEnd()
{
}

// 最大HPを取得
int CObjectBase::GetMaxHp() const
{
	return mMaxHp;
}

// 現在HPを取得
int CObjectBase::GetHp() const
{
	return mHp;
}

// ダメージを受ける
void CObjectBase::TakeDamage(int damage, CObjectBase* causer)
{
	// 既に死亡していたら、ダメージを受けない
	if (IsDeath()) return;

	// 受けたダメージが現在HP以上なら
	if (damage >= mHp)
	{
		// HPを0にして、死亡
		mHp = 0;
		Death();
	}
	// 現在HPの方が多い場合は、ダメージ分減らす
	else
	{
		mHp -= damage;
	}
}

// 死亡
void CObjectBase::Death()
{
}

// 死んでいるかどうか
bool CObjectBase::IsDeath() const
{
	// 現在HPが0ならば、死亡
	return mHp <= 0;
}

// 経路探索用のノード取得
CNavNode* CObjectBase::GetNavNode() const
{
	return mpNavNode;
}

// 攻撃がヒットしたオブジェクトを追加
void CObjectBase::AddAttackHitObj(CObjectBase* obj)
{
	mAttackHitObjects.push_back(obj);
}

// 既に攻撃がヒットしているオブジェクトかどうか
bool CObjectBase::IsAttackHitObj(CObjectBase* obj) const
{
	auto find = std::find
	(
		mAttackHitObjects.begin(),
		mAttackHitObjects.end(),
		obj
	);
	return find != mAttackHitObjects.end();
}