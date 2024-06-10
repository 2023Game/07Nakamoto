#ifndef CBILLBOARD_H
#define CBILLBOARD_H

#include "CCharacter.h"

/*
ビルボードクラス
常にカメラの方を向く四角形
*/

class CBillBoard :public CCharacter
{
public:
	//デフォトコンストラクタ
	CBillBoard();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="w">幅</param>
	/// <param name="h">高さ</param>
	CBillBoard(CVector pos, float w, float h);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="w">幅</param>
	/// <param name="h">高さ</param>
	/// <param name="priority">優先度</param>
	CBillBoard(CVector pos, float w, float h, int priority);

	/// <summary>
	/// ビルボードの設定
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="w">幅</param>
	/// <param name="h">高さ</param>
	void SetBillBoard(CVector pos, float w, float h);

	//更新
	void Update();

	//描画
	void Render();
	void Render(CMaterial* mpMaterial);

protected:
	//三角形2つ
	CTriangle mT[2];
	//マテリアル
	CMaterial mMaterial;
};

#endif // !CBILLBOARD_H
