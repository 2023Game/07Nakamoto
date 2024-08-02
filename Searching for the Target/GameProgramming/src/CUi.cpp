#include <stdio.h>
#include <glut.h>
#include "CUi.h"
#include "CCamera.h"
#include "CTarget.h"

#define TIME 120

//staticの定義
int CUi::mTime = TIME;

//デフォルトコンストラクタ
CUi::CUi()
	: mNum(0)
{
	mFont.Load("FontWhite.png", 1, 64);
}

CUi::~CUi()
{
	mTime = TIME;
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

//制限時間の管理
void CUi::Time()
{
	//60フレーム数える
	mNum++;

	//60フレームになったら
	if (mNum == 60)
	{
		//時間を1秒減らす
		mTime--;
		mNum = 0;
	}
}

int CUi::GetTime()
{
	return mTime;
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

	sprintf(buf, "TARGET:%01d", CTarget::GetNum());
	sprintf(str, "TIME:%03d", mTime);

	mFont.Draw(640, 580, 8, 16, buf);
	mFont.Draw(310, 580, 10, 20, str);

	CCamera::End();	//2D描画終了
}