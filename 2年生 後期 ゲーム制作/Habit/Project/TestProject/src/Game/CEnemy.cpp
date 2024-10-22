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
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
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
	switch (mState)
	{
	case EState::eIdle:		UpdateIdle();
	case EState::ePatrol:	UpdatePatrol();
	case EState::eChase:	UpdateChase();
	case EState::eLost:		UpdateLost();
	case EState::eAttack:	UpdateAttack();
	}

	CXCharacter::Update();

	CDebugPrint::Print("状態 : %s\n", GetStateStr(mState));
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

//状態を切り替え
void CEnemy::ChangeState(EState state)
{
	if (state == mState) return;

	mState = state;
	mStateStep = 0;

}

//待機状態時の更新処理
void CEnemy::UpdateIdle()
{
}

//巡回中の更新処理
void CEnemy::UpdatePatrol()
{
}

//追跡時の更新処理
void CEnemy::UpdateChase()
{
}

//プレイヤーを見失った時の更新処理
void CEnemy::UpdateLost()
{
}

//攻撃時の更新処理
void CEnemy::UpdateAttack()
{
}

//状態の文字列を取得
std::string CEnemy::GetStateStr(EState state) const
{
	switch (state)
	{

	}

	return std::string();
}
