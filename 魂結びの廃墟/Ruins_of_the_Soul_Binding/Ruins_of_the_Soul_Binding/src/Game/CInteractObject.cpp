#include "CInteractObject.h"

#define DEFAULT_TEXT_PATH "UI\\Interact\\interact.png"

// �R���X�g���N�^
CInteractObject::CInteractObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eInteractObject, prio, sortOrder, pause)
	, mInteractStr("���ׂ�")
	, mHp(100)
#if _DEBUG
	, mDebugName("InteractObj")
#endif
{
}

// �f�X�g���N�^
CInteractObject::~CInteractObject()
{
}

// ���ׂ����Ԃ��ǂ���
bool CInteractObject::CanInteract() const
{
	return true;
}

void CInteractObject::TakeDamage(int damege, CObjectBase* causer)
{
	if (damege >= mHp)
	{
		mHp = 0;
		Kill();
	}
	else
	{
		mHp -= damege;
	}
}

// ���ׂ���e�̃e�L�X�g��Ԃ� 
std::string CInteractObject::GetInteractStr() const
{
	return mInteractStr;
}

// ���ׂ���e�̃e�L�X�g�摜�̃p�X��Ԃ�
std::string CInteractObject::GetInteractTextPath() const
{
	return DEFAULT_TEXT_PATH;
}

// ���ׂ�UI��\��������W��Ԃ�
CVector CInteractObject::GetInteractUIPos() const
{
	return Position() + CVector(0.0f, 10.0f, 0.0f);
}

#if _DEBUG
// �f�o�b�O�\���p�̖��O���擾
std::string CInteractObject::GetDebugName() const
{
	return mDebugName;
}

// �f�o�b�O�\���p��m���O��ݒ�
void CInteractObject::SetDebugName(std::string name)
{
	mDebugName = name;
}
#endif