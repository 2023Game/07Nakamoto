#include "CDoor.h"
#include "Maths.h"

// コンストラクタ
CDoor::CDoor(CModel* model, const CVector& pos)
	: mpModel(model)
{
	Position(pos);


}

// デストラクタ
CDoor::~CDoor()
{

}

// 更新
void CDoor::Update()
{
}

// 描画
void CDoor::Render()
{
	mpModel->Render(Matrix());
}
