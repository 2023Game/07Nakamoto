#pragma once
#include <string>

struct RoomData
{
	CVector center;
	CVector size;
};

// �����N���X
class CRoom
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="center">�����̒��S�ʒu</param>
	/// <param name="size">�����̃T�C�Y</param>
	/// <param name="name">�����̖��O</param>
	CRoom(std::vector<RoomData> roomData, std::string name);
	// �f�X�g���N�^
	~CRoom();

	// �����̖��O���擾
	std::string GetName() const;
	// �����̃o�E���f�B���O�{�b�N�X��Ԃ�
	const std::vector<CBounds>& GetBounds() const;

#if _DEBUG
	// �`��
	void Render();
#endif

private:
	std::vector<RoomData> mRoomData;
	std::string mName;	// �����̖��O
	std::vector<CBounds> mBounds;	// �����̃o�E���f�B���O�{�b�N�X
};