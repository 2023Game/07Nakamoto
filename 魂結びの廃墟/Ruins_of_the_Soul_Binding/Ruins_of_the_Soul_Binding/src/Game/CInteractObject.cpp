#include "CInteractObject.h"
#include "CInteractObjectManager.h"

#define DEFAULT_TEXT_PATH "UI\\Interact\\interact.png"
#define OFFSET_POS	CVector(0.0f,10.0f,0.0f)

// �R���X�g���N�^
CInteractObject::CInteractObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eInteractObject, prio, sortOrder, pause)
	, mInteractStr("���ׂ�")
	, mInteract(true)
	, mOffSetPos(OFFSET_POS)
#if _DEBUG
	, mDebugName("InteractObj")
#endif
{
	// �Ǘ��N���X�̃��X�g�Ɏ��g��ǉ�
	CInteractObjectManager::Instance()->Add(this);
}

// �f�X�g���N�^
CInteractObject::~CInteractObject()
{
	// �Ǘ��N���X�����݂�����A���g�����X�g�����菜��
	CInteractObjectManager* mgr = CInteractObjectManager::Instance();
	if (mgr != nullptr)
	{
		mgr->Remove(this);
	}
}

// ���ׂ����Ԃ��ǂ���
bool CInteractObject::CanInteract() const
{
	return mInteract;
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
	return Position() + mOffSetPos;
}

// ���ׂ�Ƃ��ɎQ�Ƃ���I�u�W�F�N�g�̈ʒu
CVector CInteractObject::GetInteractPos() const
{
	return Position();
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