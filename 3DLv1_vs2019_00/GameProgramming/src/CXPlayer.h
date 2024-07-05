#ifndef CXCPLAYER_H
#define CXCPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CInput.h"

class CXPlayer :public CXCharacter{

public:
	//デフォルトコンストラクタ
	CXPlayer();

	//更新処理
	void Update();

	//初期設定
	void Init(CModelX* model);

private:
	//コライダの宣言
	CCollider mColShereBody;	//体
	CCollider mColShereHead;	//頭
	CCollider mColShereSword;	//剣

	CInput mInput;
};

#endif // !CXCPLAYER_H
