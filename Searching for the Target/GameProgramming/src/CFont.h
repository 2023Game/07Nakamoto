#ifndef CFONT_H
#define CFONT_H

#include "CTexture.h"

class CFont
{
public:
	/// <summary>
	/// �ǂݍ���
	/// </summary>
	/// <param name="file">�t�@�C����</param>
	/// <param name="row">�s��</param>
	/// <param name="col">��</param>
	void Load(const char* file, int row, int col);

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <param name="">��</param>
	/// <param name="h">����</param>
	/// <param name="c">����</param>
	void Draw(float x, float y, float w, float h, char c);

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <param name="w">��</param>
	/// <param name="h">����</param>
	/// <param name="c">������</param>
	void Draw(float x, float y, float w, float h, char c[]);

private:
	CTexture mTexture;
};

#endif // !CFONT.H
