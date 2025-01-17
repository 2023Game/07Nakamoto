#include "CMaterial.h"

//memset,strncpyのインクルード
#include <string.h>
#include "glut.h"

/*
strncpy(char* str1, const char* str2, int len)
コピー先str1にコピー元str2の文字をlen文字数までコピーする
*/
char* strmcpy(char* str1, const char* str2, int len)
{
	int i = 0;
	//iがlenより小さく、かつ、コピー元が終わりでない間繰り返し
	while (i < len && *str2 != '\0')
	{
		*(str1 + i) = *str2;	//コピー先にコピー元を代入
		str2++;	//コピー元を次へ
		i++;
	}
	str1[i] = '\0';	//コピー先の文字列に終わり
	return str1;	//コピー先の先頭アドレスを返却
}

//デフォルトコンストラクタ
CMaterial::CMaterial()
	:mVertexNum(0)
{
	//名前を0で埋める
	memset(mName, 0, sizeof(mName));
	//0で埋める
	memset(mDiffuse, 0, sizeof(mDiffuse));
}

//マテリアルを有効にする
void CMaterial::Enabled()
{
	//拡散校の設定
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuse);
	//テクスチャ有り
	if (mTexture.Id())
	{
		//テクスチャを使用可能にする
		glEnable(GL_TEXTURE_2D);
		//テクスチャをバインドする
		glBindTexture(GL_TEXTURE_2D, mTexture.Id());
		//アルファブレンドを有効にする
		glEnable(GL_BLEND);
		//ブレンド方法を指定
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

//マテリアルを無効にする
void CMaterial::Disabled()
{
	//テクスチャ有り
	if (mTexture.Id())
	{
		//アルファブレンドを無効
		glDisable(GL_BLEND);
		//テクスチャのバインドを解く
		glBindTexture(GL_TEXTURE_2D, 0);
		//テクスチャを無効にする
		glDisable(GL_TEXTURE_2D);
	}
}

//テクスチャの取得
CTexture* CMaterial::GetTexture()
{
	return &mTexture;
}

//マテリアルの名前の取得
char* CMaterial::GetName()
{
	return mName;
}

//マテリアルの名前を設定する
void CMaterial::SetName(char* name)
{
	strncpy(mName, name, MATERIAL_NAME_LEN);
}

//mDiffuseの配列取得
float* CMaterial::GetDiffuse()
{
	return mDiffuse;
}

void CMaterial::SetVertexNum(int num)
{
	mVertexNum = num;
}

int CMaterial::GetVertexNum()
{
	return mVertexNum;
}