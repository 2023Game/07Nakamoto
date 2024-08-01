#include "CTaskManager.h"

//�f�t�H���g�R���X�g���N�^
CTaskManager::CTaskManager()
{
	mHead.mpNext = &mTail;
	mTail.mpPrev = &mHead;
}
//�f�X�g���N�^
CTaskManager::~CTaskManager()
{
	
}
//�^�X�N�̍폜
void CTaskManager::Delete()
{
	//�擪����Ō�܂ŌJ��Ԃ�
	CTask* task = mHead.mpNext;
	while (task->mpNext)
	{
		CTask* del = task;
		//����
		task = task->mpNext;
		//mEnabled��false�Ȃ�폜
		if (del->mEnabled == false)
		{
			delete del;
		}
	}
}
//�S�C���X�^���X�폜
void CTaskManager::AllDelete()
{
	//�C�e���[�^�̐���
	std::vector<CCharacter*>::iterator itr;
	//�C�e���[�^��擪��
	itr = mpCharacters.begin();
	//�Ō�܂ŌJ��Ԃ�
	while (itr != mpCharacters.end())
	{
		//�C���X�^���X�폜
		delete* itr;
		itr = mpCharacters.erase(itr);
	}
}

//���X�g����폜
void CTaskManager::Remove(CTask* task)
{
	//�^�X�N�̑O�̎����A�^�X�N�̎��ɂ���
	task->mpPrev->mpNext = task->mpNext;
	//�^�X�N�̎��̑O���A�^�X�N�̑O�ɂ���
	task->mpNext->mpPrev = task->mpPrev;
}
//�X�V
void CTaskManager::Update()
{
	//�擪����Ō�܂ŌJ��Ԃ�
	CTask* task = mHead.mpNext;
	while (task->mpNext)
	{
		//�X�V�������Ă�
		task->Update();
		//����
		task = task->mpNext;
	}
}
//�`��
void CTaskManager::Render()
{
	//�擪����Ō�܂ŌJ��Ԃ�
	CTask* task = mHead.mpNext;
	while (task->mpNext)
	{
		//�`�揈�����Ă�
		task->Render();
		//����
		task = task->mpNext;
	}
}
//���X�g�ɒǉ�
void CTaskManager::Add(CTask* addTask)
{
	//mHead�̎����猟��
	CTask* task = mHead.mpNext;

	//�D��x�̑傫�����ɑ}������
	//�}���ʒu�̌���(�D��x���������傫���Ȃ����O)
	//mPriority >= 0�̂���
	while (addTask->mPriority < task->mPriority)
	{
		task = task->mpNext;
	}

	//mTail�̑O�ɒǉ�
	//CTask* task = &mTail;
	//addTask�̎���task
	addTask->mpNext = task;
	//addTask�̑O��task�̑O��
	addTask->mpPrev = task->mpPrev;
	//addTask�̑O�̎���addTask��
	task->mpPrev->mpNext = addTask;
	//task�̑O��addTask��
	task->mpPrev = addTask;
}

//�Փˏ���
void CTaskManager::Collision()
{
	//�擪����Ō�܂ŌJ��Ԃ�
	CTask* task = mHead.mpNext;
	while (task->mpNext) {
		//�Փˏ������Ă�
		task->Collision();
		//����
		task = task->mpNext;
	}
}

//�^�X�N�}�l�[�W���̃C���X�^���X
CTaskManager* CTaskManager::mpInstance = nullptr;

//�C���X�^���X�̎擾
CTaskManager* CTaskManager::GetInstance()
{
	//�C���X�^���X���������
	if (mpInstance == nullptr)
	{
		//�C���X�^���X�𐶐�
		mpInstance = new CTaskManager();
	}
	return mpInstance;
}
