#include "CModel.h"
#include "CVector.h"
#include "CVertex.h"
//標準出力のインクルード
#include <stdio.h>

//文字列s1とs2の比較
//s1とs2が等しければ0を
//等しくなければ0以外を返す
int strcmp(const char* s1, const char* s2)
{
	int i = 0;
	//文字が同じ間は繰り返し
	//どちらかの文字が終わりになるとループ終わり
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
	{
		i++;
	}
	//同じなら引いて0
	return s1[i] - s2[i];
}

CModel::~CModel()
{
	for (int i = 0; i < mpMaterials.size(); i++)
	{
		delete mpMaterials[i];
	}
	delete[] mpVertexes;
}

//モデルファイルの入力
//Load(モデルファイル名,マテリアルファイル名)
void CModel::Load(char* obj, char* mtl)
{
	//頂点データ保存(CVector型)
	std::vector<CVector>vertex;
	//法線データ保存(CVector型)
	std::vector<CVector>normal;
	//テクスチャマッピングの保存(CVector型)
	std::vector<CVector> uv;

	//ファイルポインタ変数の作成
	FILE* fp;
	//ファイルからデータを入力
	//入力エリアを作成する
	char buf[256];

	//ファイルのオープン
	//fopen(ファイル名,モード)
	//オープンできない時はNULを返す
	fp = fopen(mtl, "r");
	//ファイルオープンエラーの判定
	//fpがNULLの時はエラー
	if (fp == NULL)
	{
		//コンソールにエラー出力して戻る
		printf("%s file open error\n", mtl);
		return;
	}

	//マテリアルインデックス
	int idx = 0;
	//ファイルから1行入力
	//fgets(入力エリア,エリアサイズ,ファイルポインタ)
	//ファイルの最後になるとNULLを返す
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		//データを分解する
		char str[4][64] = { "","","","" };
		//文字列からデータを4つ変数へ代入する
		sscanf(buf, "%s %s %s %s", str[0], str[1], str[2], str[3]);
		//先頭がnewmtlの時、マテリアルを追加する
		if (strcmp(str[0], "newmtl") == 0)
		{
			CMaterial* pm = new CMaterial();
			//マテリアル名の設定
			pm->SetName(str[1]);
			//マテリアルの可変長配列に追加
			mpMaterials.push_back(pm);
			//配列の長さを取得
			idx = mpMaterials.size() - 1;
		}
		//先頭がmap_Kdの時、テクスチャを入力する
		else if (strcmp(str[0], "map_Kd") == 0)
		{
			mpMaterials[idx]->GetTexture()->Load(str[1]);
		}
		//先頭がKdの時、Diffseを設定する
		else if (strcmp(str[0], "Kd") == 0)
		{
			mpMaterials[idx]->GetDiffuse()[0] = atof(str[1]);
			mpMaterials[idx]->GetDiffuse()[1] = atof(str[2]);
			mpMaterials[idx]->GetDiffuse()[2] = atof(str[3]);
		}
		//先頭がdの時、a値を設定する
		else if (strcmp(str[0], "d") == 0)
		{
			mpMaterials[idx]->GetDiffuse()[3] = atof(str[1]);
		}
		
	}

	//ファイルのクローズ
	fclose(fp);

	fp = fopen(obj, "r");

	if (fp == NULL)
	{
		//コンソールにエラー出力して戻る
		printf("%s file open error\n", obj);
		return;
	}

	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		//データを分割する
		char str[4][64] = { "","","","" };
		//文字列からデータを4つ変数へ代入
		//sscanf(文字列,変換指定子,変数
		sscanf(buf, "%s %s %s %s", str[0], str[1], str[2], str[3]);
		//文字列の比較
		//strcmp(文字列1,文字列2)
		//文字列1と文字列2が同じ時0、異なる時0以外を返す
		//戦闘がvの時、頂点をvertexに追加する
		if (strcmp(str[0], "v") == 0) {
			//可変長配列vertexに追加
			//atof(文字列)文字列からfloat型の値を返す
			vertex.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
		}
		//先頭がvnの時、法線をnormalに追加する
		if (strcmp(str[0], "vn") == 0)
		{
			//可変長配列normalに追加
			//atof(文字列)文字列からfloat型の値を返す
			normal.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
		}
		//先頭がvtの時、uvに追加する
		if (strcmp(str[0], "vt") == 0)
		{
			//可変長配列uvに追加する
			//atof(文字列) 文字列からfloat型の値を返す
			uv.push_back(CVector(atof(str[1]), atof(str[2]), 0.0));
		}
		//先頭がusemtlの時、マテリアルデックスを取得する
		else if (strcmp(str[0], "usemtl") == 0)
		{
			//可変長配列を後から比較
			for (idx = mpMaterials.size() - 1; idx > 0; idx--)
			{
				//同じ名前のマテリアルがあればループ終了
				if (strcmp(mpMaterials[idx]->GetName(), str[1]) == 0)
				{
					break;	//ループ終了
				}
			}
		}
		//先頭がfの時、三角形を作成して追加する
		else if (strcmp(str[0], "f") == 0) {
			//頂点と法線の番号作成
			int v[3], n[3];
			//テクスチャマッピングの有無を判定
			if (strstr(str[1], "//"))
			{
				//頂点と法線の番号取得
				sscanf(str[1], "%d//%d", &v[0], &n[0]);
				sscanf(str[2], "%d//%d", &v[1], &n[1]);
				sscanf(str[3], "%d//%d", &v[2], &n[2]);
				//三角形作成
				CTriangle t;
				t.SetVertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
				t.SetNormal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
				//マテリアル番号設定
				t.SetMaterialIdx(idx);
				//可変長配列mTrianglesに三角形を追加
				mTriangles.push_back(t);
			}
			else
			{
				//テクスチャマッピング有り
				int u[3];	//テクスチャマッピングの番号
				//頂点と法線の番号取得とマッピングの番号取得
				sscanf(str[1], "%d/%d/%d", &v[0], &u[0], &n[0]);
				sscanf(str[2], "%d/%d/%d", &v[1], &u[1], &n[1]);
				sscanf(str[3], "%d/%d/%d", &v[2], &u[2], &n[2]);
				//三角形作成
				CTriangle t;
				t.SetVertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
				t.SetNormal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
				//テクスチャマッピングの設定
				t.SetUV(uv[u[0] - 1], uv[u[1] - 1], uv[u[2] - 1]);
				//マテリアル番号設定
				t.SetMaterialIdx(idx);
				//可変長配列mTrianglesに三角形を追加
				mTriangles.push_back(t);
			}
		}
		
	}
	//ファイルのクローズ
	fclose(fp);

	CreateVertexBuffer();
}

//描画
void CModel::Render() 
{
	//可変長配列の要素数だけ繰り返し
	for (int i = 0; i < mTriangles.size(); i++)
	{
		//マテリアルの適用
		mpMaterials[mTriangles[i].GetMaterialIdx()]->Enabled();
		//可変長配列に添え字でアクセスする
		mTriangles[i].Render();
		//マテリアルを無効
		mpMaterials[mTriangles[i].GetMaterialIdx()]->Disabled();
	}
}

//描画
void CModel::Render(const CMatrix& m)
{
	//行列の退避
	glPushMatrix();
	//合成行列を掛ける
	glMultMatrixf(m.GetM());
	//頂点座標の位置を設定
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(CVertex), (void*)&mpVertexes[0].mPosition);
	//法線ベクトルの位置を設定
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(CVertex), (void*)&mpVertexes[0].mNormal);
	//テクスチャマッピングの位置を設定
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(CVertex), (void*)&mpVertexes[0].mTextureCoords);

	int first = 0; //描画位置
	//マテリアル毎に描画する
	for (size_t i = 0; i < mpMaterials.size(); i++)
	{
		//マテリアルを適用する
		mpMaterials[i]->Enabled();
		//描画位置からのデータで三角形を描画します
		glDrawArrays(GL_TRIANGLES, first, mpMaterials[i]->GetVertexNum());
		//マテリアルを無効にする
		mpMaterials[i]->Disabled();
		//描画位置を移動
		first += mpMaterials[i]->GetVertexNum();
	}
	//行列を戻す
	glPopMatrix();

	//頂点座標の配列を無効にする
	glDisableClientState(GL_VERTEX_ARRAY);
	//法線の配列を無効にする
	glDisableClientState(GL_NORMAL_ARRAY);
	//テクスチャマッピングの配列を無効にする
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

const std::vector<CTriangle>& CModel::Triangles() const
{
	return mTriangles;
}

//頂点の配列
void CModel::CreateVertexBuffer()
{
	mpVertexes = new CVertex[mTriangles.size() * 3];
	int idx = 0;
	for (int i = 0; i < mpMaterials.size(); i++)
	{
		for (int j = 0; j < mTriangles.size(); j++)
		{
			if (i == mTriangles[j].GetMaterialIdx())
			{
				mpMaterials[i]->SetVertexNum(mpMaterials[i]->GetVertexNum() + 3);
				mpVertexes[idx].mPosition = mTriangles[j].GetV0();
				mpVertexes[idx].mNormal = mTriangles[j].GetN0();
				mpVertexes[idx++].mTextureCoords = mTriangles[j].GetmU0();
				mpVertexes[idx].mPosition = mTriangles[j].GetV1();
				mpVertexes[idx].mNormal = mTriangles[j].GetN1();
				mpVertexes[idx++].mTextureCoords = mTriangles[j].GetmU1();
				mpVertexes[idx].mPosition = mTriangles[j].GetV2();
				mpVertexes[idx].mNormal = mTriangles[j].GetN2();
				mpVertexes[idx++].mTextureCoords = mTriangles[j].GetmU2();
			}
		}
	}
}