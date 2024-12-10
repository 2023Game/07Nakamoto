#include "CInteractObject.h"

// �R���X�g���N�^
CInteractObject::CInteractObject(ETaskPriority prio, int sortOrder = 0, ETaskPauseType pause)
	: CObjectBase(ETag::eInteractObject, prio, sortOrder, pause)
	, mInteractStr("���ׂ�")
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

// ���ׂ���e�̃e�L�X�g��Ԃ� 
std::string CInteractObject::GetInteractStr() const
{
	return std::string();
}

#if _DEBUG
// �f�o�b�O�\���p�̖��O���擾
std::string CInteractObject::GetDebugName() const
{
	return mInteractStr;
}

// �f�o�b�O�\���p��m���O��ݒ�
void CInteractObject::SetDebugName(std::string name)
{
}
#endif