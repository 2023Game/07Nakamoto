#include "CXEnemy.h"

//�f�t�H���g�R���X�g���N�^
CXEnemy::CXEnemy()
	: mColShereBody(this, nullptr, CVector(0.5f, -1.0f, 0.0f), 1.0f)
	, mColShereHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)
	, mColShereSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f)
	, mColShereSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f)
	, mColShereSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f)
{
	
}

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColShereBody.Matrix(&mpCombinedMatrix[1]);
	//��
	mColShereHead.Matrix(&mpCombinedMatrix[1]);
	//��
	mColShereSword0.Matrix(&mpCombinedMatrix[26]);
	mColShereSword1.Matrix(&mpCombinedMatrix[26]);
	mColShereSword2.Matrix(&mpCombinedMatrix[26]);
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