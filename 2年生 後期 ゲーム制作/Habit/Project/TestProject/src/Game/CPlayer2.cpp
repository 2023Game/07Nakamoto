#include "CPlayer2.h"
#include "CEffect.h"
#include "CCollisionManager.h"

// プレイヤーのアニメーションデータのテーブル
const CPlayer2::AnimData CPlayer2::ANIM_DATA[] =
{
	{ "",											true,	0.0f	},	// Tポーズ
	{ "Character\\Player2\\anim\\pico_idle.x",		true,	181.0f	},	// 待機
	{ "Character\\Player2\\anim\\pico_walk.x",		true,	30.0f	},	// 歩行

};

// コンストラクタ
CPlayer2::CPlayer2()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
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
	ChangeAnimation(EAnimType::eIdle);
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
