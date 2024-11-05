#include "CPlayer2.h"
#include "CInput.h"

// プレイヤーのインスタンス
CPlayer2* CPlayer2::spInstatnce = nullptr;

// プレイヤーのアニメーションデータのテーブル
const CPlayer2::AnimData CPlayer2::ANIM_DATA[] =
{
	{ "",											true,	0.0f	},	// Tポーズ
	{ "Character\\Player2\\anim\\pico_idle.x",		true,	181.0f	},	// 待機
	{ "Character\\Player2\\anim\\pico_walk.x",		true,	30.0f	},	// 歩行
	{ "Character\\Player2\\anim\\pico_run.x",		true,	22.0f	},	// 走行
	{ "Character\\Player2\\anim\\pico_jump.x",		true,	51.0f	},	// ジャンプ
	{ "Character\\Player2\\anim\\pico_crawl.x",		true,	55.0f	},	// 這う
	{ "Character\\Player2\\anim\\pico_sneak.x",		true,	51.0f	},	// しゃがみ移動
	{ "Character\\Player2\\anim\\pico_crouch_and_pick_up.x",		true,	180.0f	},	// しゃがんで拾う

	
};

#define PLAYER_HEIGHT	16.0f	
#define MOVE_SPEED		10.0f	// 歩く速度
#define RUN_SPEED		20.0f	// 走る速度

// コンストラクタ
CPlayer2::CPlayer2()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
{
	// インスタンスの設定
	spInstatnce = this;

	// モデルデータの取得
	CModelX* model = CResourceManager::Get<CModelX>("Player2");

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
}

// デストラクタ
CPlayer2::~CPlayer2()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}

	spInstatnce = nullptr;
}

// インスタンスのポインタの取得
CPlayer2* CPlayer2::Instance()
{
	return spInstatnce;
}

// 更新処理
void CPlayer2::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 状態に合わせて、変更処理を切り替える
	switch (mState)
	{
	case EState::eIdle:
		break;
	
	}

	// キャラクターの更新
	CXCharacter::Update();
}

CVector CPlayer2::CalcMoveVec() const
{
	return CVector();
}

void CPlayer2::UpdateIdle()
{
}

void CPlayer2::UpdateJumpStart()
{
}

void CPlayer2::UpdateMove()
{
}

// 衝突判定
void CPlayer2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}


// 描画処理
void CPlayer2::Render()
{
	CXCharacter::Render();
}

// アニメーションの切り替え
void CPlayer2::ChangeAnimation(EAnimType type)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return;
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.framelength);
}
