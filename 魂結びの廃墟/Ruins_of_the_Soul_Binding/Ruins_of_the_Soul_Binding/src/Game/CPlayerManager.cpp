#include "CPlayerManager.h"
#include "CPlayerBase.h"
#include <assert.h>
#include "CInput.h"

#define RESERVED_CAPACITYE 5	// ���X�g�̏����e��

CPlayerManager* CPlayerManager::spInstance = nullptr;

// �v���C���[�Ǘ��N���X�̃C���X�^���X���擾
CPlayerManager* CPlayerManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CPlayerManager::CPlayerManager()
	:CTask(ETaskPriority::ePlayer, 0, ETaskPauseType::eGame)
	, mCurrentIndex(0)
{
	assert(spInstance == nullptr);
	spInstance = this;

	// ���X�g�̏����e�ʂ̐ݒ�
	mPlayers.reserve(RESERVED_CAPACITYE);
}

// �f�X�g���N�^
CPlayerManager::~CPlayerManager()
{
	spInstance = nullptr;
	mPlayers.clear();
}

// �v���C���[�̒ǉ�
void CPlayerManager::AddPlayer(CPlayerBase* player)
{
	mPlayers.push_back(player);
}

// �v���C���[����菜��
void CPlayerManager::RemovePlayer(CPlayerBase* player)
{
	auto find = std::find(mPlayers.begin(), mPlayers.end(), player);
	if (find == mPlayers.end()) return;
	mPlayers.erase(find);
}

// ����L������؂�ւ���
void CPlayerManager::ChangePlayer()
{
	int size = mPlayers.size();
	mCurrentIndex = (mCurrentIndex + 1) % size;

	// �S�v���C���[�ɁA���̃v���C���[�𑀍삷�邩�ǂ�����ݒ�
	for (int i = 0; i < size; i++)
	{
		mPlayers[i]->SetOperate(i == mCurrentIndex);
	}
}

// ���삵�Ă��Ȃ��L������ҋ@��Ԃɂ���
void CPlayerManager::SetInactivePlayersWait()
{

}

// �X�V
void CPlayerManager::Update()
{
	if (CInput::PushKey('C'))
	{
		ChangePlayer();
	}
	if (CInput::PushKey('Q'))
	{

	}
}
