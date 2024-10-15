#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"

// プレイヤーのアニメーションデータのテーブル
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// Tポーズ
	//{ "Character\\Player2\\anim\\idle.x",			true,	122.0f	},	// 待機
	//{ "Character\\Model\\vampire.x",		false,	91.0f	},	// 攻撃

};

// コンストラクタ
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
{
	//モデルデータの取得
	CModelX* model = CResourceManager::Get<CModelX>("Enemy");

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
	ChangeAnimation(EAnimType::eTPose);
}

CEnemy::~CEnemy()
{
}

//更新処理
void CEnemy::Update()
{
	CXCharacter::Update();
}

//描画処理
void CEnemy::Render()
{
	CXCharacter::Render();
}

//アニメーションの切り替え
void CEnemy::ChangeAnimation(EAnimType type)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return;
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.framelength);
}
