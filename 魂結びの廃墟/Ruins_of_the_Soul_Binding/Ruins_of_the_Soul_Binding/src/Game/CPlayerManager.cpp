#include "CPlayerManager.h"
#include "CPlayerBase.h"
#include <assert.h>
#include "CInput.h"
#include "CIcon.h"
#include "CPlayer2.h"

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

	// �A�C�R���̕ύX
	ETag tag = mPlayers[mCurrentIndex]->Tag();
	if (tag == ETag::eCat)
	{
		// �L�̃A�C�R����ݒ�
		CIcon::Instance()->SetIcon((int)CIcon::EIcon::eCat);
	}
	else if (tag == ETag::ePlayer)
	{
		CPlayer2* player = dynamic_cast<CPlayer2*>(mPlayers[mCurrentIndex]);

		if (player->GatActingTogether())
		{
			// �L�̃A�C�R����ݒ�
			CIcon::Instance()->SetIcon((int)CIcon::EIcon::eTogether);
		}
		else
		{
			// �����̃A�C�R����ݒ�
			CIcon::Instance()->SetIcon((int)CIcon::EIcon::ePlayer);
		}
	}
}

// �v���C���[�̃��X�g���擾
const std::vector<CPlayerBase*>& CPlayerManager::GetPlayers() const
{
	return mPlayers;
}

// �X�V
void CPlayerManager::Update()
{
	if (CInput::PushKey('C'))
	{
		ChangePlayer();

		// �A�C�R���̕ύX
		ETag tag = mPlayers[mCurrentIndex]->Tag();
		if (tag == ETag::eCat)
		{
			// �L�̃A�C�R����ݒ�
			CIcon::Instance()->SetIcon((int)CIcon::EIcon::eCat);
		}
		else if (tag == ETag::ePlayer)
		{
			// �����̃A�C�R����ݒ�
			CIcon::Instance()->SetIcon((int)CIcon::EIcon::ePlayer);
		}
	}
}
