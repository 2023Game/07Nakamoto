#ifndef CFONT_H
#define CFONT_H

#include "CTexture.h"

class CFont
{
public:
	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="file">ファイル名</param>
	/// <param name="row">行数</param>
	/// <param name="col">列数</param>
	void Load(const char* file, int row, int col);

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <param name="">幅</param>
	/// <param name="h">高さ</param>
	/// <param name="c">文字</param>
	void Draw(float x, float y, float w, float h, char c);

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <param name="w">幅</param>
	/// <param name="h">高さ</param>
	/// <param name="c">文字列</param>
	void Draw(float x, float y, float w, float h, char c[]);

private:
	CTexture mTexture;
};

#endif // !CFONT.H
