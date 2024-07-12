#ifndef CMODELX_H		//インクルードガード
#define CMODELX_H
#include <vector>		//vectorクラスのインクルード(動的配列)
#include "CMatrix.h"	//マトリクスクラスのインクルード

class CMesh;			//CMeshクラスの宣言
class CModelX;			//CModelXクラスに宣言
class CModelXFrame;		//CModelXFeameクラスの宣言
class CMaterial;		//CMaterialクラスの宣言
class CSkinWeights;		//スキンウェイトクラス
class CAnimationSet;	//アニメーションセットクラス
class CAnimation;		//アニメーションクラス
class CAnimationKey;	//アニメーションキークラス

#define MODEL_FILE "res\\ラグナ.x"	//入力ファイル名

//領域開放をマクロ化
#define SAFE_DELETE_ARRAY(a){if(a) delete[] a;a=nullptr;}

//配列のサイズ取得をマクロ化
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

class CMesh {
public:
	//コンストラクタ
	CMesh();
	//デストラクタ
	~CMesh();

	void AnimateVertex(CMatrix*);

	//頂点にアニメーション適用
	void AnimateVertex(CModelX* model);

	//スキンウェイトにフレーム番号を設定する
	void SetSkinWeightFrameIndex(CModelX* model);

	//読み込み処理
	void Init(CModelX* model);
	void Render();
private:
	//テクスチャ座標データ
	float* mpTextureCoords;

	CVector* mpAnimateVertex;	//アニメーション用頂点
	CVector* mpAnimateNormal;	//アニメーション用法線

	//スキンウェイト
	std::vector<CSkinWeights*> mSkinWeights;

	int mMaterialNum;	//マテリアル数
	int mMaterialIndexNum;	//マテリアル番号数(面数)
	int* mpMaterialIndex;	//マテリアル番号
	std::vector<CMaterial*> mMaterial;	//マテリアルデータ

	int mNormalNum;		//法線数
	CVector* mpNormal;	//法線ベクトル

	int mFaceNum;		//面数
	int* mpVertexIndex; //面を構成する頂点インデックス

	int mVertexNum;		//頂点数
	CVector* mpVertex;	//頂点データ
};

/*
CModelX
Xファイル形式の3Dモデルデータをプログラムで認識する
*/
class CModelX{
	friend CAnimationSet;
	friend CModelXFrame;
	friend CAnimation;
public:
	void AnimateVertex(CMatrix*);
	//マテリアルの検索
	CMaterial* FindMaterial(char* name);
	//マテリアル配列の取得
	std::vector<CMaterial*>& Material();

	//頂点にアニメーションを適用
	void AnimateVertex();
	//スキンウェイトのフレーム番号設定
	void SetSkinWeightFrameIndex();
	//フレーム名に該当するフレームのアドレスを返す
	CModelXFrame* FindFrame(char* name);
	//トークンが無くなったらtrue
	bool EOT();
	//描画
	void Render();
	//mTokenのポインタを返す
	char* Token();
	//デストラクタ
	~CModelX();
	//ノードの読み飛ばし
	void SkipNode();
	//単語の取り出し
	char* GetToken();
	CModelX();
	//ファイル読み込み
	void Load(char* file);
	
	void AnimateFrame();

	std::vector<CModelXFrame*>& Frames();

	std::vector<CAnimationSet*> &AnimationSet();

private:
	//マテリアル配列
	std::vector<CMaterial*> mMaterial;
	//アニメーションセットの配列
	std::vector<CAnimationSet*>mAnimationSet;

	std::vector<CModelXFrame*>mFrame;	//フレームの配列
	//cが区切り文字ならtrueを返す
	bool IsDelimiter(char c);
	char* mpPointer;		//読み込み位置
	char mToken[1024];	//取り出した単語領域
};

//CModelXFrameクラスの定義
class CModelXFrame {
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
public:
	const CMatrix& CombinedMatrix();

	//合成行列の作成
	void AnimateCombined(CMatrix* parent);
	//デフォルトコンストラクタ
	CModelXFrame();
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame();
	int Index();
	void Render();
private:
	CMatrix mCombinedMatrix;	//合成行列
	CMesh* mpMesh;	//Meshデータ
	std::vector<CModelXFrame*>mChild;	//子フレームの配列
	CMatrix mTransformMatrix;	//変換行列
	char* mpName;	//フレーム名前
	int mIndex;		//フレーム番号
};

class CSkinWeights {
	friend CModelX;
	friend CMesh;
public:
	CSkinWeights(CModelX* model);
	~CSkinWeights();
	const int& FrameIndex();
	const CMatrix& Offset();
private:
	char* mpFrameName;	//フレーム名
	int mFrameIndex;	//フレーム番号
	int mIndexNum;		//頂点番号数
	int* mpIndex;		//頂点番号配列
	float* mpWeight;	//頂点ウェイト配列
	CMatrix mOffset;	//オフセットマトリックス
};

/*
CAnimationSet
アニメーションセット
*/
class CAnimationSet {
	friend CModelX;
	
public:
	CAnimationSet(CModelX* model);
	~CAnimationSet();

	float Time();
	float MaxTime();

	void Time(float time);		//時間の設定
	void Weight(float weight);	//重みの設定

	void AnimateMatrix(CModelX* model);
	std::vector < CAnimation*>& Animation();
private:
	float mTime;		//現在時間
	float mWeight;		//重み
	float mMaxTime;		//最大時間

	//アニメーション
	std::vector<CAnimation*> mAnimation;
	//アニメーションセット名
	char* mpName;
};

/*
CAnimation
アニメーションクラス
*/
class CAnimation {
	friend CAnimationSet;
	friend CModelX;
public:
	CAnimation(CModelX* model);
	~CAnimation();
private:
	int mKeyNum;		//キー数（時間数）
	CAnimationKey* mpKey;	//キーの配列

	char* mpFrameName;	//フレーム名
	int mFrameIndex;	//フレーム番号

};

/*
CAnimarionKey
アニメーションキークラス
*/
class CAnimationKey {
	friend CAnimation;
	friend CAnimationSet;
private:
	//時間
	float mTime;
	//行列
	CMatrix mMatrix;
};

#endif