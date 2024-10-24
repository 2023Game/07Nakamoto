#ifndef CPLAYER_H
#define CPLAYER_H
#include "CXCharacter.h"

/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayer2 : public CXCharacter
{
public:
	//コンストラクタ
	CPlayer2();
	//デストラクタ
	~CPlayer2();

	//更新処理
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
		eWalk,		//歩行
		eRun,		//走行
		eJump,		//ジャンプ
		eCrawl,		//這う
		eSneak,		//しゃがみ移動
		eCrouch_up, //しゃがんで拾う

		Num
	};
	//アニメーション切り替え
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

#endif // !CPLAYER_H

