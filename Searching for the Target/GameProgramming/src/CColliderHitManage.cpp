#include "CColliderHitManager.h"

CColliderHitManager::CColliderHitManager()
	:mCol(ECollider::EFLOOR)
{

}

CColliderHitManager* CColliderHitManager::mpInstance = nullptr;

//�C���X�^���X�̎擾
CColliderHitManager* CColliderHitManager::GetInstance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CColliderHitManager();
	}
	return mpInstance;
}

std::vector<CCollider*> CColliderHitManager::mpHitColliders;

//�v���C���[�Ɠ������Ă���R���C�_���ϒ��z��ɒǉ�
void CColliderHitManager::AddColliders(CCollider* o)
{
	//���ɓ����ĂȂ����m�F
	for (size_t i = 0; i < mpHitColliders.size(); i++)
	{
		if (mpHitColliders[i] == o)
			return;
	}
	//�����A�h���X���Ȃ���Βǉ�
	mpHitColliders.push_back(o);
}

//�R���C�_�[�̎�ނɂ���Ĕ��肷��R���C�_�[���P���o��
CCollider *CColliderHitManager::HitColliderSerch()
{
	if (!mpHitColliders.size())
	{
		return nullptr;
	}

	//��̐��A���̐�
	int s = 0, f = 0;

	for (size_t i = 0; i < mpHitColliders.size(); i++)
	{
		if (mpHitColliders[i]->GetTag() == CCollider::ETag::ESLOPE)
		{
			s++;
		}
		else if (mpHitColliders[i]->GetTag() == CCollider::ETag::ENULL)
		{
			f++;
		}
	}

	printf("s:%2d f:%2d\n", s, f);

	if (s > 0 && f == 0)
	{
		mCol = ECollider::ESLOPE;
	}
	else if (s == 0 && f > 0)
	{
		mCol = ECollider::EFLOOR;
	}
	/*
	else if (s > 0 && f > 0 && mCol == ECollider::EFLOOR)
	{
		mCol = ECollider::EFLOOR;
	}
	else if (s > 0 && f > 0 && mCol == ECollider::ESLOPE)
	{
		mCol = ECollider::ESLOPE;
	}
	*/
	//mCol�����ō�ɓ����������ɍ��킹��
	//if (mCol == ECollider::EFLOOR)
	//{
		for (size_t i = 0; i < mpHitColliders.size(); i++)
		{
			if (mpHitColliders[i]->GetTag() == CCollider::ETag::ESLOPE)
			{
				//CCollider::GetNormal(mpHitColliders[i]);
				return mpHitColliders[i];
			}
		}
	//}
	if (mCol == ECollider::EFLOOR) 
	{
		//CCollider::GetNormal(mpHitColliders[0]);
		//mCol����ŏ��ɓ��������珰�ɍ��킹��
		return mpHitColliders[0];
	}	

	return nullptr;
}

float x = 0, y = 180, z = 0;

//�Փ˔���
void CColliderHitManager::HitCollision(CCollider* m, CCollider* o)
{
	if (o != nullptr)
	{
		//�Ζʂ̊p�x������
		CCollider::Slope(m, o, &ajustRote);

		//printf("%10f %10f %10f\n", ajustRote.GetX(), ajustRote.GetY(), ajustRote.GetZ());

		//��ɓ���������Ζʂɍ��킹�ĉ�]
		m->GetParent()->SetRotation(ajustRote);

		/*
		if (m->GetParent()->GetRotation().GetX() < ajustRote.GetX())
			x = m->GetParent()->GetRotation().GetX() - 1;
		else if (m->GetParent()->GetRotation().GetX() > ajustRote.GetX())
			x = m->GetParent()->GetRotation().GetX() + 1;

		if (m->GetParent()->GetRotation().GetY() < ajustRote.GetY())
			y = m->GetParent()->GetRotation().GetY() - 1;
		else if (m->GetParent()->GetRotation().GetY() > ajustRote.GetY()+180)
			y = m->GetParent()->GetRotation().GetY() + 1;

		if (m->GetParent()->GetRotation().GetZ() < ajustRote.GetZ())
			z = m->GetParent()->GetRotation().GetZ() - 1;
		else if (m->GetParent()->GetRotation().GetZ() > ajustRote.GetZ())
			z = m->GetParent()->GetRotation().GetZ() + 1;

		m->GetParent()->SetRotation(CVector(x, y, z));
		*/
	}
}

//�C���l�̎擾
CVector CColliderHitManager::GetAjustRote()
{
	return ajustRote;
}

//��������R���C�_�̎�ނ̎擾
CColliderHitManager::ECollider CColliderHitManager::GetCol()
{
	return mCol;
}

//�폜
void CColliderHitManager::Delete()
{
	mpHitColliders.clear();
}