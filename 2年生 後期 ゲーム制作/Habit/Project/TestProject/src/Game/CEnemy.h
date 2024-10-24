#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CModel.h"

/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CXCharacter
{
public:
	// コンストラクタ
	CEnemy();
	//デストラクタ
	~CEnemy();

	// 更新処理
	void Update() override;
	
	//描画処理
	void Render() override;

private:
	//アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eIdle,		//待機
		eWalk,		//歩行
		eRun,		//走行
		eJump,		//ジャンプ
		eJumpAttack,//ジャンプ攻撃
		eAttack,	//攻撃

		Num			//アニメーションの種類の数
	};
	//アニメーションの切り替え
	void ChangeAnimation(EAnimType type);

	//アニメーションデータ
	struct AnimData
	{
		std::string path;	//アニメーションデータのパス
		bool loop;			//ループするかどうか
		float framelength;	//アニメーションのフレーム数
	};
	//アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	enum class EState
	{
		eIdle,		// 待機
		ePatrol,	// 巡回中
		eChase,		// プレイヤーを追いかける
		eLost,		// プレイヤーを見失う
		eAttack,	// プレイヤー攻撃
	};

	//状態を切り替え
	void ChangeState(EState state);

	//待機状態時の更新処理
	void UpdateIdle();
	//巡回中の更新処理
	void  UpdatePatrol();
	//追跡時の更新処理
	void UpdateChase();
	//プレイヤーを見失った時の更新処理
	void UpdateLost();
	//攻撃時の更新処理
	void UpdateAttack();

	//状態の文字列を取得
	std::string GetStateStr(EState state) const;

	EState mState;	//敵の状態
	int mStateStep;	//状態内のステップ管理用
	float mElapsedTime;	//経過時間計測用

};
#endif
