#include "CDoor.h"
#include "Maths.h"

// �R���X�g���N�^
CDoor::CDoor(CModel* model, const CVector& pos)
	: mpModel(model)
{
	Position(pos);


}

// �f�X�g���N�^
CDoor::~CDoor()
{

}

// �X�V
void CDoor::Update()
{
}

// �`��
void CDoor::Render()
{
	mpModel->Render(Matrix());
}
