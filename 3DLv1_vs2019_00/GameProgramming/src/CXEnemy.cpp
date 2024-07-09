#include "CXEnemy.h"

//�f�t�H���g�R���X�g���N�^
CXEnemy::CXEnemy()
	: mColShereBody(this, nullptr, CVector(), 0.5f)
	, mColShereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColShereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
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

void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	switch (m->Type())
	{
	case CCollider::EType::ESPHERE:

		//����̃R���C�_�[�^�C�v������
		if (o->Type() == CCollider::EType::ESPHERE)
		{
			//����̃R���C�_�̃^�O������
			if (o->Tag() == CCollider::ETag::ESWORD)
			{
				//�����̃R���C�_�̃^�O���̂�������
				if (m->Tag() == CCollider::ETag::EBODY)
				{
					//�Փ˂��Ă��邩����
					if (CCollider::Collision(m, o))
					{
						//30�t���[�������ă_�E�����A�J��Ԃ��Ȃ�
						ChangeAnimation(11, false, 30);
					}
				}
			}
		}
		break;
	}
}