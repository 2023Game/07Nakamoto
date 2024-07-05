#include "CXEnemy.h"

//�f�t�H���g�R���X�g���N�^
CXEnemy::CXEnemy()
	: mColShereBody(this, nullptr, CVector(), 0.5f)
	, mColShereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColShereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f)
{
	
}

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColShereBody.Matrix(&mpCombinedMatrix[8]);
	//��
	mColShereHead.Matrix(&mpCombinedMatrix[11]);
	//��
	mColShereSword.Matrix(&mpCombinedMatrix[21]);
}