#ifndef CCHARACTER_H
#define CCHARACTER_H

//変換行列クラスのインクルード
#include "CTransform.h"
//モデルクラスのインクルード
#include "CModel.h"
#include "CTask.h"

class CCollider;

/*
キャラクタークラス
ゲームキャラクタの基本的な機能を定義
*/
class CCharacter :public CTransform ,public CTask 
{
public:
	//タイプ
	enum class ETag
	{
		ENULL,		//なし
		ESLOPE,		//斜面
		EBULLET,	//弾
		ETARGET,	//的
		ESWITCH,	//スイッチ
	};

	//デフォルトコンストラクタ
	CCharacter();
	//デストラクタ
	~CCharacter();

	//コンストラクタ
	CCharacter(int priority);

	//有効フラグを取得
	bool GetEnabled();

	/// <summary>
	/// モデルの設定
	/// </summary>
	/// <param name="m">モデルクラスのポインタ</param>
	void SetModel(CModel* m);

	//タイプの設定
	CCharacter::ETag SetTag(ETag tag);
	//タイプの取得
	CCharacter::ETag GetTag();

	//描画処理
	void Render();

	//衝突処理
	virtual void Collision(CCollider* m, CCollider* o) {}

protected:
	CModel* mpModel;	//モデルのポインタ
	CCharacter::ETag mTag;

private:
	
};

#endif // !CCHARACTER_H

