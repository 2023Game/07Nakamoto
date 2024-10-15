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

		eTPose,		//Tポーズ
		eIdle,		//待機
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

};
#endif
