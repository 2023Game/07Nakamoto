#include "CMashroom.h"
#include "CColliderSphere.h"

#define ANIM_PATH "Character\\Enemy\\Mushroom\\Anim\\"

// 敵のアニメーションデータのテーブル
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f	},	// Tポーズ
	{ ANIM_PATH"idle.x",		true,	41.0f,	1.0f	},	// 待機
	{ ANIM_PATH"attack1.x",		true,	26.0f,	1.0f	},	// 頭突き攻撃

};

// コンストラクタ
CMashroom::CMashroom()
{
	// ゲージのオフセット位置を設定
	mGaugeOffsetPos = CVector(0.0f, 10.0f, 0.0f);

	// 敵を初期化
	InitEnemy("Mushroom", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

}

// デストラクタ
CMashroom::~CMashroom()
{
}

// 更新
void CMashroom::Update()
{
	// 敵のベースクラスの更新
	CEnemy::Update();


}
