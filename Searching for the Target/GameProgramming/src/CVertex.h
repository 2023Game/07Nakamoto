#ifndef CVERTEX_H
#define CVERTEX_H

#include "CVector.h"
/*
頂点データクラス
頂点毎にデータをまとめる
*/
class CVertex
{
public:
	//位置
	CVector mPosition;
	//法線
	CVector mNormal;
	//テクスチャマッピング
	CVector mTextureCoords;
};

#endif // !CVERTEX_H

