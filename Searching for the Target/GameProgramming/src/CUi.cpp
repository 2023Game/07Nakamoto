#include <stdio.h>
#include <glut.h>
#include "CUi.h"
#include "CCamera.h"

int CUi::mPoint = 0;

//デフォルトコンストラクタ
CUi::CUi()
	: mNum(0)
	, mTime(120)
{
	mFont.Load("FontWhite.png", 1, 64);
}

void CUi::State()
{
	CCamera::Start(0, 800, 0, 600);	//2D描画開始
	mFont.Draw(36, 300, 18, 36, "START PUSH ENTER KEY!");
	CCamera::End();	//2D描画終了
}

void CUi::Clear()
{
	CCamera::Start(0, 800, 0, 600);	//2D描画開始
	mFont.Draw(36, 360, 18, 36, "     GAME CREAR!!");
	//mFont.Draw(36, 300, 18, 36, "START PUSH ENTER KEY!");
	CCamera::End();	//2D描画終了
}

//時間の設定
void CUi::Time()
{
	mNum++;

	if (mNum == 60)
	{
		mTime--;
		mNum = 0;
	}
}
//ポイントを加算
void CUi::AddPoint()
{
	mPoint++;
}
//ポイントの取得
int CUi::GetPoint()
{
	return mPoint;
}

//描画
void CUi::Render()
{
	CCamera::Start(0, 800, 0, 600);	//2D描画開始
	//描画色の設定(緑色の半透明)
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	//文字列編集エリアの作成
	char buf[64];
	char str[16];

	sprintf(buf, "POINT:%01d", mPoint);
	sprintf(str, "TIME:%03d", mTime);

	mFont.Draw(640, 580, 8, 16, buf);
	mFont.Draw(310, 580, 10, 20, str);

	CCamera::End();	//2D描画終了
}