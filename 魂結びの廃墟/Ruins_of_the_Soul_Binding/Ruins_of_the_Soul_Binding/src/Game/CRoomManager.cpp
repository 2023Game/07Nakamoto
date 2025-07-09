#include "CRoomManager.h"
#include "CPlayerManager.h"
#include "CPlayerBase.h"

#if _DEBUG
#include "CDebugInput.h"
#endif

CRoomManager* CRoomManager::spInstance = nullptr;

// �R���X�g���N�^
CRoomManager::CRoomManager()
	: CTask(ETaskPriority::eRoom,0,ETaskPauseType::eGame)
#if _DEBUG
	, mIsShowRoom(false)
#endif
{

}

// �f�X�g���N�^
CRoomManager::~CRoomManager()
{
}

// �C���X�^���X���擾
CRoomManager* CRoomManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CRoomManager();
	}

	return spInstance;
}

// ���������X�g�ɒǉ�
void CRoomManager::Add(CRoom* room)
{
	mRooms.push_back(room);
}

// ���������X�g�����菜��
void CRoomManager::Remove(CRoom* room)
{
	auto result = std::remove(mRooms.begin(), mRooms.end(), room);
	mRooms.erase(result);
}

// ��X�V
void CRoomManager::LateUpdate()
{
	auto players = CPlayerManager::Instance()->GetPlayers();
	for (CPlayerBase* player : players)
	{
		const CBounds& playerBounds = player->GetBounds();
		CRoom* inRoom = nullptr;
		for (CRoom* room : mRooms)
		{
			const CBounds& roomBounds = room->GetBounds();
			// �����ƃv���C���[m�̃o�E���f�B���O�{�b�N�X���G��Ă�����
			if (CBounds::Intersect(roomBounds, playerBounds))
			{
				// �����Ă��镔����ݒ�
				inRoom = room;
				break;
			}
		}

		player->SetRoom(inRoom);
	}
}

#if _DEBUG
// ������\�����邩�ǂ���
bool CRoomManager::IsShowRoom() const
{
	return mIsShowRoom;
}

// �`��
void CRoomManager::Render()
{
	// [SHIFT]+[1]�L�[�ŕ����̕\���t���O��؂�ւ�
	if (CDebugInput::Key(VK_SHIFT) && CDebugInput::PushKey('1'))
	{
		mIsShowRoom = !mIsShowRoom;
	}

	// �\���t���O�������Ă��Ȃ���΁A�������Ȃ�
	if (!mIsShowRoom)	return;

	// ���X�g�ɓo�^���Ă��镔����`��
	for (CRoom* room : mRooms)
	{
		room->Render();
	}
}
#endif
