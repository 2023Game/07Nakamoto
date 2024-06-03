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
class CCharacter :public CTransform ,public CTask {
public:
	//デフォルトコンストラクタ
	CCharacter();
	//デストラクタ
	~CCharacter();

	//コンストラクタ
	CCharacter(int priority);

	/// <summary>
	/// モデルの設定
	/// </summary>
	/// <param name="m">モデルクラスのポインタ</param>
	void SetModel(CModel* m);

	//描画処理
	void Render();

	//衝突処理
	virtual void Collision(CCollider* m, CCollider* o) {}

protected:
	CModel* mpModel;	//モデルのポインタ

private:

};

#endif // !CCHARACTER_H

