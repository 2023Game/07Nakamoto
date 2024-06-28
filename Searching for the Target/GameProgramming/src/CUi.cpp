#include <stdio.h>
#include <glut.h>
#include "CUi.h"
#include "CCamera.h"

CUi::CUi()
	: mPosY(0)
	//, mRotX(0)
	, mTime(0)
{
	mFont.Load("FontWhite.png", 1, 64);
}

void CUi::SetPosY(float f)
{
	mPosY = f;
}

void CUi::SetRotX(float f)
{
	//mRotX = f;
}
//時間の設定
void CUi::SetTime(int time)
{
	mTime = time;
}
//描画
void CUi::Render()
{
	CCamera::Start(0, 800, 0, 600);	//2D描画開始
	//描画色の設定(緑色の半透明)
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	//文字列編集エリアの作成
	char buf[64];

	sprintf(buf, "PY:%7.2f", mPosY);

	mFont.Draw(500, 300, 8, 16, buf);

	CCamera::End();	//2D描画終了
}