#pragma once
#include "CObjectBase.h"

class CInteractObject : public CObjectBase
{
public:
	// �R���X�g���N�^
	CInteractObject(
		ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// �f�X�g���N�^
	virtual ~CInteractObject();

	// ���ׂ����Ԃ��ǂ���
	virtual bool CanInteract() const;
	// ���ׂ�i�p����Ŏ����j
	virtual void Interact() = 0;

	// ���ׂ���e�̃e�L�X�g��Ԃ� 
	std::string GetInteractStr() const;
	// ���ׂ���e�̃e�L�X�g�摜�̃p�X��Ԃ�
	virtual std::string GetInteractTextPath() const;
	// ���ׂ�UI��\��������W��Ԃ�
	virtual CVector GetInteractUIPos() const;

#if _DEBUG
	// �f�o�b�O�\���p�̖��O���擾
	std::string GetDebugName() const;
	// �f�o�b�O�\���p��m���O��ݒ�
	void SetDebugName(std::string name);
#endif

protected:
	std::string mInteractStr;	// ���ׂ���e�̃e�L�X�g

#if _DEBUG
	std::string mDebugName;		// �f�o�b�O�\���p�̖��O
#endif
};