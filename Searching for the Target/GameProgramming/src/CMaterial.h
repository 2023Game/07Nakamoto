#ifndef CMATERIAL_H
#define CMATERIAL_H
#define MATERIAL_NAME_LEN 64	//名前の長さ

#include "CTexture.h"

/*
マテリアルクラス
マテリアルのデータを扱う
*/
class CMaterial
{
public:
	//デフォルトコンストラクタ
	CMaterial();
	//マテリアルを有効にする
	void Enabled();
	//マテリアルを無効にする
	void Disabled();
	//テクスチャの取得
	CTexture* GetTexture();
	//マテリアル名前の取得
	char* GetName();

	/// <summary>
	/// マテリアルの名前の取得
	/// </summary>
	/// <param name="name">マテリアルの名前</param>
	void SetName(char* name);

	/// 拡散校の取得
	float* GetDiffuse();

private:
	//テクスチャ
	CTexture mTexture;
	//マテリアル名
	char mName[MATERIAL_NAME_LEN + 1];
	//拡散校の色 RGBA
	float mDiffuse[4];
};

#endif // !CMATERIAL_H
