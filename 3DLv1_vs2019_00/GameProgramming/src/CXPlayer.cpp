#include "CXPlayer.h"

void CXPlayer::Update()
{
	if (AnimationIndex() != 3 && AnimationIndex() != 4)
	{
		//時計回りに回転
		if (mInput.Key('A'))
		{
			//Y軸回転値に2加算する
			mRotation = mRotation + CVector(0.0f, 2.0f, 0.0f);
		}
		//反時計回りに回転
		if (mInput.Key('D'))
		{
			//Y軸回転値に2減算する
			mRotation = mRotation - CVector(0.0f, 2.0f, 0.0f);
		}

		//歩くモーションに切り替え、前方に進む
		if (mInput.Key('W'))
		{
			//歩くアニメーションに切り替える
			ChangeAnimation(1, true, 60);
			//前方に0.1進む
			mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
		}
		//待機モーション
		else if (!mInput.Key('W'))
		{
			//待機アニメーションに切り替える
			ChangeAnimation(0, true, 60);
		}
	}
	//攻撃モーション
	if (mInput.Key(VK_SPACE) && AnimationIndex() != 4)
	{
		//攻撃アニメーションに切り替える
		ChangeAnimation(3, false, 30);
	}
	//攻撃モーション2に切り替える
	else if (AnimationIndex() == 3 && IsAnimationFinished() == true)
	{
		ChangeAnimation(4, false, 30);
	}
	//待機モーションに戻す
	else if (AnimationIndex() == 4 && IsAnimationFinished() == true)
	{
		ChangeAnimation(0, true, 60);
	}

	CXCharacter::Update();
}