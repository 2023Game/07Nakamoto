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
	spInstance = this;
}

// �f�X�g���N�^
CRoomManager::~CRoomManager()
{
	spInstance = nullptr;

	// �����̃f�[�^��S�č폜
	auto itr = mRooms.begin();
	while (itr != mRooms.end())
	{
		CRoom* room = *itr;
		delete room;
		itr = mRooms.erase(itr);
	}
}

// �C���X�^���X���擾
CRoomManager* CRoomManager::Instance()
{
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

// �w�肵���I�u�W�F�N�g�������Ă��镔����Ԃ�
CRoom* CRoomManager::GetCurrentRoom(CObjectBase* obj) const
{
	const CBounds& objBounds = obj->GetBounds();
	CRoom* inRoom = nullptr;
	for (CRoom* room : mRooms)
	{
		const std::vector<CBounds>& roomBounds = room->GetBounds();
		for (const CBounds& bounds : roomBounds)
		{
			// �����ƃI�u�W�F�N�g�̃o�E���f�B���O�{�b�N�X���G��Ă�����
			if (CBounds::Intersect(bounds, objBounds))
			{
				// �����Ă��镔����Ԃ�
				return room;
			}
		}
	}

	return nullptr;
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
