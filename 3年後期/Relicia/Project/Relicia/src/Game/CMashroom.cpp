#include "CMashroom.h"
#include "CColliderCapsule.h"

#define ANIM_PATH "Character\\Enemy\\Mushroom\\Anim\\"

#define BODY_HEIGHT 13.0f
#define BODY_RADIUS 5.0f

// 敵のアニメーションデータのテーブル
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f	},	// Tポーズ
	//{ ANIM_PATH"idle.x",		true,	41.0f,	1.0f	},	// 待機
	//{ ANIM_PATH"attack1.x",		true,	26.0f,	1.0f	},	// 頭突き攻撃

};

// コンストラクタ
CMashroom::CMashroom()
{
	// ゲージのオフセット位置を設定
	mGaugeOffsetPos = CVector(0.0f, 10.0f, 0.0f);

	// 敵を初期化
	InitEnemy("Mushroom", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eTPose);

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	// フィールドと、プレイヤーの攻撃コライダーとヒットするように設定
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer });
	mpBodyCol->SetCollisionLayers({ ELayer::eFloor, ELayer::eWall, ELayer::ePlayer, ELayer::eAttackCol });
}

// デストラクタ
CMashroom::~CMashroom()
{
}

// 衝突処理
void CMashroom::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ベースの衝突処理を呼び出す
	CEnemy::Collision(self, other, hit);
}

// 更新
void CMashroom::Update()
{
	// 敵のベースクラスの更新
	CEnemy::Update();


}
