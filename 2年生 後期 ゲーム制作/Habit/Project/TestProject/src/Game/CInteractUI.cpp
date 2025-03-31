#include "CInteractUI.h"
#include "CCamera.h"
#include <glut.h>
#include "CImage3D.h"
#include "Maths.h"
#include "CInteractObject.h"

#define WORLD_UNIT_PER_PIXEL 32.0f
#define IMAGE_SPACE 0.2f	// 調べるUIの画像同士の間のサイズ
#define IMAGE_OFFSET -2.3f	// 調べるUIの画像のオフセット値
#define IMAGE_ALPHA 0.8f	// 調べるUIの画像のアルファ値

// コンストラクタ
CInteractUI::CInteractUI()
	: CObjectBase(ETag::eUI, ETaskPriority::eUI3D, 0, ETaskPauseType::eGame)
	, mpButtonImage(nullptr)
	, mpTextImage(nullptr)
{
	// インタラクトボタンのイメージを読み込み
	mpButtonImage = new CImage3D
	(
		"UI\\BottonUI\\E.png",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpButtonImage->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	mpButtonImage->SetBillboard(true);
	mpButtonImage->SetDepthTest(false);
	mpButtonImage->SetAlignment(EAlignment::eMiddleRight);
	mpButtonImage->SetAlpha(IMAGE_ALPHA);

	// 調べる内容テキストのイメージを読み込み
	mpTextImage = new CImage3D
	(
		"",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpTextImage->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	mpTextImage->SetBillboard(true);
	mpTextImage->SetDepthTest(false);
	mpTextImage->SetAlignment(EAlignment::eMiddleLeft);
	mpTextImage->SetAlpha(IMAGE_ALPHA);
}

// デストラクタ
CInteractUI::~CInteractUI()
{
	// 読み込んだイメージを削除
	SAFE_DELETE(mpButtonImage);
	SAFE_DELETE(mpTextImage);
}

// 調べるUIを表示
void CInteractUI::Show(CInteractObject* obj)
{
	// 調べるオブジェクトごとの内容テキストのイメージを読み込み
	std::string path = obj->GetInteractTextPath();
	mpTextImage->Load(path);

	// 調べるオブジェクトの座標に移動
	Position(obj->GetInteractUIPos());

	// UIを表示
	SetShow(true);
}

// 調べるUIを非表示
void CInteractUI::Hide()
{
	SetShow(false);
}

// 更新
void CInteractUI::Update()
{
	mpButtonImage->Update();
	mpTextImage->Update();
}

// 描画
void CInteractUI::Render()
{
	// 中心座標を取得
	CVector pos = Position();
	// カメラの横方向ベクトルを取得
	CCamera* cam = CCamera::CurrentCamera();
	CVector camSide = CVector::Cross(CVector::up, cam->VectorZ());

	// 操作ボタンのイメージの座標を設定して、描画
	mpButtonImage->Position(pos + camSide * (IMAGE_OFFSET -IMAGE_SPACE * 0.5f));
	mpButtonImage->Render();

	// 調べる内容テキストのイメージの座標を設定して、描画
	mpTextImage->Position(pos + camSide * (IMAGE_OFFSET + IMAGE_SPACE * 0.5f));
	mpTextImage->Render();
}
