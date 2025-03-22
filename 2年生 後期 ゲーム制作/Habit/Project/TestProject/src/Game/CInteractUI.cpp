#include "CInteractUI.h"
#include "CCamera.h"
#include <glut.h>
#include "CImage3D.h"
#include "Maths.h"

#define WORLD_UNIT_PER_PIXEL 32.0f

// コンストラクタ
CInteractUI::CInteractUI(CObjectBase* owner)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI3D, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mpInteractUI(nullptr)
{
	// インタラクトボタンのイメージを読み込み
	mpInteractUI = new CImage3D
	(
		"UI\\BottonUI\\E.png",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpInteractUI->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	mpInteractUI->SetDepthMask(true);
	mGaugeSize = mpInteractUI->GetSize();

}

// デストラクタ
CInteractUI::~CInteractUI()
{
	// 読み込んだイメージを削除
	SAFE_DELETE(mpInteractUI);

	// 持ち主に削除されたことを伝える
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// 持ち主を設定
void CInteractUI::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// 更新
void CInteractUI::Update()
{
	mpInteractUI->Update();
}

// 描画
void CInteractUI::Render()
{
	// 行列の保存
	glPushMatrix();

	// 常にカメラの方向を向かせる
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	// インタラクトボタンの描画
	mpInteractUI->Render();

	// 行列を戻す
	glPopMatrix();
}
