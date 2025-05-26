#include "CPlayerManager.h"
#include "CPlayerBase.h"
#include <assert.h>
#include "CInput.h"

CPlayerManager* CPlayerManager::spInstance = nullptr;

// �v���C���[�Ǘ��N���X�̃C���X�^���X���擾
CPlayerManager* CPlayerManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CPlayerManager::CPlayerManager()
	:CTask(ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mCurrentIndex(0)
{
	assert(spInstance == nullptr);
	spInstance = this;
}

// �f�X�g���N�^
CPlayerManager::~CPlayerManager()
{
	spInstance = nullptr;

	auto itr = mPlayers.begin();
	auto end = mPlayers.end();
	while (itr != end)
	{
		CPlayerBase* del = *itr;
		itr = mPlayers.erase(itr);
		delete del;
	}
}

// �v���C���[�̒ǉ�
void CPlayerManager::AddPlayer(CPlayerBase* player)
{
	mPlayers.push_back(player);
}

// �v���C���[����菜��
void CPlayerManager::RemovePlayer(CPlayerBase* player)
{
	mPlayers.remove(player);
}

// ����L������؂�ւ���
void CPlayerManager::ChangePlayer()
{
	mCurrentIndex = (mCurrentIndex + 1) % mPlayers.size();
}

// �X�V
void CPlayerManager::Update()
{
	mPlayers[mCurrentIndex]->Update();
}
