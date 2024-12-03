#include "CPushButtonManager.h"

// �^�X�N�}�l�[�W���̃C���X�^���X
CPushButtonManager* CPushButtonManager::mpInstance = nullptr;

CPushButtonManager* CPushButtonManager::Instance()
{
	// �C���X�^���X��������΁A�V������������
	if (mpInstance == nullptr)
	{
		mpInstance = new CPushButtonManager();
	}
	return mpInstance;
}

// �C���X�^���X�̔j��
void CPushButtonManager::ClearInstance()
{
	delete mpInstance;
	mpInstance = nullptr;
}

// �R���X�g���N�^
CPushButtonManager::CPushButtonManager()
{
}

// �f�X�g���N�^
CPushButtonManager::~CPushButtonManager()
{
	std::list<CTask*>& list = mPushButtonList;
	// �擪����Ō�܂ŌJ��Ԃ�
	auto itr = list.begin();
	auto end = list.end();
	while (itr != end)
	{
		// �폜�^�X�N���L�����Ă���
		CTask* del = *itr;
		itr = list.erase(itr);
		delete del;
	}
}

// �w�肵���{�^�������X�g�ɒǉ�
void CPushButtonManager::Add(CTask* pushbutton)
{
	auto it = std::find(mPushButtonList.begin(), mPushButtonList.end(), pushbutton);
	if (it != mPushButtonList.end())return;

	// ���X�g�̍Ō���ɒǉ�
	mPushButtonList.push_back(pushbutton);
}

// �w�肵���{�^�������X�g�����菜��
void CPushButtonManager::Remove(CTask* pushbutton)
{
	mPushButtonList.remove(pushbutton);
}

//// �{�^���̃��X�g���擾
//const std::list<CTask*>& CPushButtonManager::GetTasks() const
//{
//	return mPushButtonList;
//}