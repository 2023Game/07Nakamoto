#include "CBillBoard.h"
#include "CApplication.h"

//デフォルトコンストラクタ
CBillBoard::CBillBoard()
{

}

//コンストラクタ
CBillBoard::CBillBoard(CVector pos, float w, float h)
{
	SetBillBoard(pos, w, h);
}

//コンストラクタ
CBillBoard::CBillBoard(CVector pos, float w, float h, int priority)
	: CCharacter(priority)
{
	SetBillBoard(pos, w, h);
}

//ビルボードの設定
void CBillBoard::SetBillBoard(CVector pos, float w, float h)
{
	//位置
	mPosition = pos;
	//大きさの設定
	mScale = CVector(1.0f, 1.0f, 1.0f);
	//三角形の頂点座標設定
	mT[0].SetVertex(CVector(w, h, 0.0f), CVector(-w, -h, 0.0f), CVector(w, -h, 0.0f));
	mT[1].SetVertex(CVector(-w, h, 0.0f), CVector(-w, -h, 0.0f), CVector(w, h, 0.0f));
	//法線をZ軸方向
	mT[0].SetNormal(CVector(0.0f, 0.0f, 1.0f));
	mT[1].SetNormal(CVector(0.0f, 0.0f, 1.0f));
	//色を白色を設定
	mMaterial.GetDiffuse()[0] = 1.0f;
	mMaterial.GetDiffuse()[1] = 1.0f;
	mMaterial.GetDiffuse()[2] = 1.0f;
	mMaterial.GetDiffuse()[3] = 1.0f;
}

//更新
void CBillBoard::Update()
{
	//行列の更新
	CTransform::Update();
}

//描画
void CBillBoard::Render()
{
	Render(&mMaterial);
}

//描画(マテリアルのポインタ)
void CBillBoard::Render(CMaterial* mpMaterial)
{
	//行列の保存
	glPushMatrix();
	//拡縮、回転、移動させる
	glMultMatrixf((CApplication::ModelViewInverse() * mMatrix).GetM());
	//ライトオフ
	glDisable(GL_LIGHTING);
	//描画色の設定
	glColor4fv(mpMaterial->GetDiffuse());
	//マテリアル適用
	mpMaterial->Enabled();
	//三角形の描画
	mT[0].Render();
	mT[1].Render();
	//マテリアル解除
	mpMaterial->Disabled();
	//ライトオン
	glEnable(GL_LIGHTING);
	//行列を戻す
	glPopMatrix();
	
}