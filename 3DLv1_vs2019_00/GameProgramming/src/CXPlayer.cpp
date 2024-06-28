#include "CXPlayer.h"

void CXPlayer::Update()
{
	if (mInput.Key('A'))
	{
		//Y軸回転値に2加算する
		mRotation = mRotation + CVector(0.0f, 2.0f, 0.0f);
	}

	if (mInput.Key('D'))
	{
		//Y軸回転値に2減算する
		mRotation = mRotation - CVector(0.0f, 2.0f, 0.0f);
	}

	if (mInput.Key('W'))
	{
		//歩くアニメーションに切り替える
		ChangeAnimation(1, true, 60);
		//前方に0.1進む
		mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
	}
	else if (!mInput.Key('W'))
	{
		//待機アニメーションに切り替える
		ChangeAnimation(0, true, 60);
	}

	CXCharacter::Update();
}