#ifndef CPLAYER_H
#define CPLAYER_H

#include "CCharacter.h"
#include "CInput.h"
#include "CBullet.h"

/*
プレイヤークラス
キャラクタクラスを継承
*/

class CPlayer :public CCharacter {
public:
	//デフォルトコンストラクタ
	CPlayer() {}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡縮</param>
	CPlayer(const CVector& pos, 
		const CVector& rot, const CVector& scale);
	//更新処理
	void Update();

private:
	CInput mInput;
};

#endif // !CPLAYER_H
