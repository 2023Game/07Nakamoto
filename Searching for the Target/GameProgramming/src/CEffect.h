#ifndef CEFFECT_H
#define CEFFECT_H

#include "CBillBoard.h"

/*
エフェクトクラス
テクスチャのアニメーション
*/

class CEffect :public CBillBoard
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="w">幅</param>
	/// <param name="h">高さ</param>
	/// <param name="texture">テクスチャ名</param>
	/// <param name="row">行数</param>
	/// <param name="col">列数</param>
	/// <param name="fps">1コマあたりのフレーム数</param>
	CEffect(const CVector& pos, float w, float h, char* texture, int row = 1, int col = 1, int fps = 1);
	//行数、列数、フレーム数はデフォルト引数で呼び出し時省略可能

	//更新
	void Update();
	//描画
	void Render();

private:
	//行数
	int mRows;
	//列数
	int mCols;
	//1コマのフレーム数
	int mFps;
	//フレームカウンタ
	int mFrame;
	//マテリアル
	static CMaterial sMaterial;
};

#endif // !CEFFECT_H

