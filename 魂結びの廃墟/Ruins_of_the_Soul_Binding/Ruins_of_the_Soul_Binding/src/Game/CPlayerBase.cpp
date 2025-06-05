#include "CPlayerBase.h"
#include "CInteractObject.h"
#include "Maths.h"
#include "CPlayerManager.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CWarrok.h"

#define GRAVITY 0.0625f	// 重力加速度
#define FOV_ANGLE		 60.0f		// 視野範囲の角度

// コンストラクタ
CPlayerBase::CPlayerBase()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(0)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
	, mGroundNormal(CVector::up)
	, mpBodyCol(nullptr)
	, mFovAngle(FOV_ANGLE)
	, mpSearchCol(nullptr)
	, mIsOperate(false)
	, mpCamera(nullptr)
{
	CPlayerManager::Instance()->AddPlayer(this);

	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);
}

// デストラクタ
CPlayerBase::~CPlayerBase()
{
	// コライダー削除
	SAFE_DELETE(mpBodyCol);

	// プレイヤー管理クラスが存在したら、プレイヤーリストから自身を取り除く
	CPlayerManager* pm = CPlayerManager::Instance();
	if (pm != nullptr)
	{
		pm->RemovePlayer(this);
	}
}

// 操作するかどうか設定
void CPlayerBase::SetOperate(bool operate)
{
	mIsOperate = operate;
	// このプレイヤーを操作するのであれば、
	if (mIsOperate)
	{
		// このプレイヤー用のカメラに切り替える
		mpCamera->SetCurrent(true);

		//TODO:フェードアウトの処理
	}
}

// 操作中のプレイヤーかどうか
bool CPlayerBase::IsOperate() const
{
	return mIsOperate;
}

// 操作中のカメラのポインタを設定
void CPlayerBase::SetCamera(CCamera* camera)
{
	mpCamera = camera;
}

// 衝突処理
void CPlayerBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体のコライダーの衝突判定
	if (self == mpBodyCol)
	{
		// フィールドとの衝突
		if (other->Layer() == ELayer::eField)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// 衝突した地面が床か天井かを内積で判定
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// 内積の結果がプラスであれば、床と衝突した
			if (dot >= 0.0f)
			{
				// 落下などで床に上から衝突した時（下移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// 接地した
				mIsGrounded = true;
				// 接地した地面の法線を記憶しておく
				mGroundNormal = hit.adjust.Normalized();

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}
			}
			// 内積の結果がマイナスであれば、天井と衝突した
			else
			{
				// ジャンプなどで天井に下から衝突した時（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// 壁と衝突した場合
		else if (other->Layer() == ELayer::eWall 
			|| other->Layer() == ELayer::eInteractObj
			|| other->Layer() == ELayer::eDoor)
		{
			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
		// 敵と衝突した場合
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
	}
	// 調べるオブジェクトの探知コライダーとの当たり判定
	else if (self == mpSearchCol)
	{
		CInteractObject* obj = dynamic_cast<CInteractObject*>(other->Owner());
		if (obj != nullptr)
		{
			// 調べるオブジェクトの削除フラグが立っていなかったら
			if (!obj->IsKill())
			{
				// 衝突した調べるオブジェクトをリストに追加
				mNearInteractObjs.push_back(obj);
			}
		}
	}
}

// プレイヤーの初期化
void CPlayerBase::InitPlayer(std::string path, const std::vector<AnimData>* pAnimData)
{
	// アニメーションデータテーブルを設定
	mpAnimData = pAnimData;

	// モデルデータを読み込み
	CModelX* model = CResourceManager::Get<CModelX>(path);

	// テーブル内のアニメーションデータを読み込み
	int size = mpAnimData->size();
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = (*mpAnimData)[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}

	// CXCharacterの初期化
	Init(model);
}

// 状態切り替え
void CPlayerBase::ChangeState(int state)
{
	// 同じ状態の場合は切り替えない
	if (state == mState) return;

	// 状態を変更して、状態関連の変数の初期化
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// アニメーション切り替え
void CPlayerBase::ChangeAnimation(int type, bool restart)
{
	if (mpAnimData == nullptr) return;
	if (!(0 <= type && type < mpAnimData->size())) return;
	AnimData data = (*mpAnimData)[type];
	CXCharacter::ChangeAnimation
	(
		type,
		data.loop,
		data.frameLength,
		restart
	);
	CXCharacter::SetAnimationSpeed(data.speed);
}

CInteractObject* CPlayerBase::GetNearInteractObj() const
{
	// 一番近くの調べるオブジェクトのポインタ格納用
	CInteractObject* nearObj = nullptr;
	float nearDist = 0.0f;	// 現在一番近くにある調べるオブジェクトまでの距離
	CVector pos = Position();	// プレイヤーの座標を取得
	// 探知範囲内の調べるオブジェクトを順番に調べる
	for (CInteractObject* obj : mNearInteractObjs)
	{
		if (!obj->IsDeath())
		{
			// 現在調べられない状態であれば、スルー
			if (!obj->CanInteract()) continue;
		}

		// オブジェクトの座標を取得
		CVector objPos = obj->Position();
		// プレイヤーからオブジェクトまでのベクトルを求める
		CVector vec = objPos - pos;
		vec.Y(0.0f);	// オブジェクトとの高さの差を考慮しない

		// ベクトルを正規化して長さを1にする
		CVector dir = vec.Normalized();
		// 自身の正面方向ベクトルを取得
		CVector forward = VectorZ();
		// プレイヤーとまでのベクトルと
		// 自身の正面方向ベクトルの内積を求めて角度を出す
		float dot = CVector::Dot(dir, forward);
		// 視野角度のラジアンを求める
		float angleR = Math::DegreeToRadian(mFovAngle);
		// 求めた内積と視野角度で、視野範囲内か判断する
		if (dot < cosf(angleR)) continue;

		float dist = (obj->Position() - pos).LengthSqr();
		// 一番最初の調べるオブジェクトか、
		// 求めた距離が現在の一番近いオブジェクトよりも近い場合は、
		if (nearObj == nullptr || dist < nearDist)
		{
			// 一番近いオブジェクトを更新
			nearObj = obj;
			nearDist = dist;
		}
	}
	return nearObj;
}

// 更新
void CPlayerBase::Update()
{
	// キャラクターの更新
	CXCharacter::Update();

	// 調べるオブジェクトのリストをクリア
	mNearInteractObjs.clear();
}

// 描画
void CPlayerBase::Render()
{
	CXCharacter::Render();
}

