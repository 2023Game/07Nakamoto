#include "CPlayer2.h"
#include "CEffect.h"
#include "CCollisionManager.h"

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

// コンストラクタ
CPlayer2::CPlayer2()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)

{
	//モデルデータの取得
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
	ChangeAnimation(EAnimType::eSneak);
}

CPlayer2::~CPlayer2()
{
}

//更新処理
void CPlayer2::Update()
{
	CXCharacter::Update();
}

//描画処理
void CPlayer2::Render()
{
	CXCharacter::Render();
}

//アニメーションの切り替え
void CPlayer2::ChangeAnimation(EAnimType type)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return;
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.framelength);
}
