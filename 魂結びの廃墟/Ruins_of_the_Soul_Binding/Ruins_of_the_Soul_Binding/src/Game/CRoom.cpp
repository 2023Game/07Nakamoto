#include "CRoom.h"
#include "CRoomManager.h"
#include "Primitive.h"

// �o�E���f�B���O�{�b�N�X�̃T�C�Y�𕔉��̃T�C�Y���珬�������钷��
#define BOUNDS_REDUCE_DIST 3.0f

// �R���X�g���N�^
CRoom::CRoom(std::vector<RoomData> roomData, std::string name)
	: mRoomData(roomData)
	, mName(name)
{
	// �����Ǘ��N���X�̃��X�g�Ɏ��M��o�^
	CRoomManager::Instance()->Add(this);

	for (const RoomData& data : mRoomData)
	{
		// �����̃o�E���f�B���O�{�b�N�X�̐ݒ�
		CVector boundsSize = data.size;;
		boundsSize.X(boundsSize.X() - BOUNDS_REDUCE_DIST * 2.0f);
		boundsSize.Z(boundsSize.Z() - BOUNDS_REDUCE_DIST * 2.0f);
		CBounds bounds;
		bounds.SetPos(data.center, boundsSize);
		mBounds.push_back(bounds);
	}
}

// �f�X�g���N�^
CRoom::~CRoom()
{
	// �����Ǘ��N���X�̃��X�g���玩�g����菜��
	CRoomManager::Instance()->Remove(this);
}

// �����̖��O���擾
std::string CRoom::GetName() const
{
	return mName;
}

// �����̃o�E���f�B���O�{�b�N�X��Ԃ�
const std::vector<CBounds>& CRoom::GetBounds() const
{
	return mBounds;
}

// �`��
void CRoom::Render()
{
	int count = mRoomData.size();
	for (int i = 0; i < count; i++)
	{
		const RoomData& data = mRoomData[i];

		CColor color = CColor::cyan;
		// �����̃T�C�Y�Ń��C���[�{�b�N�X��`��
		Primitive::DrawWireBox(data.center, data.size, color);
		// �����̃o�E���f�B���O�{�b�N�X�̃T�C�Y��`��
		color.A(0.5f);
		Primitive::DrawBox(mBounds[i].Center(), mBounds[i].Size(), color);
	}
}
