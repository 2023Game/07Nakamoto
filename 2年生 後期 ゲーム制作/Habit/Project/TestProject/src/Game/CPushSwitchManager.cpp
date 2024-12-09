#include "CPushSwitchManager.h"
#include "CPushSwitch.h"
#include <assert.h>

// �X�C�b�`�Ǘ��N���X�̃C���X�^���X
CPushSwitchManager* CPushSwitchManager::spInstance = nullptr;

// �X�C�b�`�Ǘ��N���X�̎擾
CPushSwitchManager* CPushSwitchManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CPushSwitchManager::CPushSwitchManager()
	: CTask(ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
{
	assert(spInstance == nullptr);
	spInstance = this;

}

// �f�X�g���N�^
CPushSwitchManager::~CPushSwitchManager()
{
	spInstance = nullptr;
	// �擪����Ō�܂ŌJ��Ԃ�
	auto itr = mPushSwitchList.begin();
	auto end = mPushSwitchList.end();
	while (itr != end)
	{
		// �폜�^�X�N���L�����Ă���
		CTask* del = *itr;
		itr = mPushSwitchList.erase(itr);
		delete del;
	}
}

// �w�肵���X�C�b�`�����X�g�ɒǉ�
void CPushSwitchManager::AddSwitch(CPushSwitch* pushswitch)
{
	// ���X�g�̍Ō���ɒǉ�
	mPushSwitchList.push_back(pushswitch);
}

// �w�肵���X�C�b�`�����X�g�����菜��
void CPushSwitchManager::RemoveSwitch(CPushSwitch* pushswitch)
{
	// �ŏ����폜����
	// �ŏ�+���l�̏ꏊ���폜�ɕύX����
	mPushSwitchList.erase(mPushSwitchList.begin());
}

