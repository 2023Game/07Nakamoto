#include "CEffect.h"

CMaterial CEffect::sMaterial;	//マテリアル、テクスチャ

//コンストラクタ
CEffect::CEffect(const CVector& pos, float w, float h, char* texture
	, int row, int col, int fps)
	: CBillBoard(pos, w, h), mRows(row), mCols(col), mFps(fps), mFrame(0)
{
	//テクスチャを読んでいない場合は読む
	if (sMaterial.GetTexture()->Id() == 0)
	{
		sMaterial.GetTexture()->Load(texture);
		sMaterial.GetDiffuse()[0] = 1.0f;
		sMaterial.GetDiffuse()[1] = 1.0f;
		sMaterial.GetDiffuse()[2] = 1.0f;
		sMaterial.GetDiffuse()[3] = 1.0f;
	}
}

//更新
void CEffect::Update()
{
	//コマ数の計算
	int frame = mFrame++ / mFps;
	frame %= mRows * mCols;
	//UV左
	float left = 1.0f / mCols * (frame % mCols);
	//UV右
	float right = left + 1.0f / mCols;
	//UV上
	float top = 1.0f - 1.0f / mRows * (frame / mCols);
	//UV下
	float bot = top - 1.0f / mRows;
	//テクスチャマッピング
	mT[0].SetUV(CVector(right, top, 0.0f), CVector(left, bot, 0.0f),
		CVector(right, bot, 0.0f));
	mT[1].SetUV(CVector(left, top, 0.0f), CVector(left, bot, 0.0f),
		CVector(right, top, 0.0f));
	//ビルボード更新
	CBillBoard::Update();
}

//描画
void CEffect::Render()
{
	glDisable(GL_DEPTH_TEST);	//深度テスト無効
	CBillBoard::Render(&sMaterial);
	glEnable(GL_DEPTH_TEST);	//深度テスト有効
}