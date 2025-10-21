#include "CDungeonMap.h"
#include "Maths.h"

// �������镔���̕��ƍ����̍ŏ��l
#define MIN_ROOM_SIZE 5

// �������镔���̕��Ɖ��s�̍ő�l
// ���T�C�Y��-2���ė��[�̗���󂯂������̍ő�l��ݒ肷��
// ���T�C�Y�������̏ꍇ�͊�ɂ��邽�߂�-3�����ĕ����̍ő�l�Ƃ���
#define MAX_ROOM_WIDTH (ROOM_WIDTH % 2 == 0 ? ROOM_WIDTH - 3 : ROOM_WIDTH - 2)
#define MAX_ROOM_HEIGHT (ROOM_HEIGHT % 2 == 0 ? ROOM_HEIGHT - 3 : ROOM_HEIGHT -2)

// �����̔��̍ő吔
#define MAX_DOOR 3

// �R���X�g���N�^
CDungeonMap::CDungeonMap()
{
	// ���̏�����
	Initialize(ROOM_HEIGHT, ROOM_WIDTH);
	// ���ɕύX������i�[�p���X�g�̏�����
	mEntranceCandidates.clear();

	// ���̃f�[�^�̐錾
	RoomInfo info;
	SetRoomParameters(info);

	// �����̏�����
	InitializeSection();
	// �@�����̏��̐ݒ�
	CreateRoomFloor(info);
	// �A�����̕ǂ̐ݒ�
	CreateRoomWall(info);
	// �B�����̎l���̒���ݒ�
	CreateRoomPillar(info);
	// �ݒ肵�����������X�g�Ɋi�[
	mRooms.push_back(info);

	// �C�����̏o�����̐ݒ�
	//CreateRoomEntrance(info);

#if _DEBUG
	// ���^�C���̃f�o�b�O�\��
	PrintSection();
#endif
}

// �f�X�g���N�^
CDungeonMap::~CDungeonMap()
{
}

// ��惊�X�g�̏�����
void CDungeonMap::Initialize(int width, int height)
{
	mMapData.resize(height);
	for (int y = 0; y < height; ++y)
	{
		// �f�t�H���gTile�ŏ�����
		mMapData[y].resize(width, Tile()); 
	}
}

// �^�C���̏����擾
const CDungeonMap::Tile& CDungeonMap::GetTile(int x, int y) const
{
	return mMapData[y][x];
}

// �����̕������X�g���擾
std::vector<CDungeonMap::RoomInfo> CDungeonMap::GetRooms() const
{
	return mRooms;
}

// �����̏o�����̃��X�g���擾
std::vector<CDungeonMap::Point> CDungeonMap::GetEntrances() const
{
	return mEntrances;
}

// �^�C���^�C�v�̐ݒ�
void CDungeonMap::SetTileType(int x, int y, TileType type)
{
	if (y >= 0 && y < mMapData.size() && x >= 0 && x < mMapData[y].size())
		mMapData[y][x].typeId = type;
}

// �����̏o�����̐ݒ�(�o�����������p���w��)
void CDungeonMap::CreateRoomEntrance(Direction dir)
{
	const RoomInfo& info = mRooms[0];

	if (dir == Direction::eSouth)
	{
		// �����̏�[�̕ǁiy���W���傫�����j
		int index = Math::Rand(info.x + 2, info.x + info.w - 3);
		mMapData[info.y][index].typeId = TileType::eEntrance;
	}
	else if (dir == Direction::eNorth)
	{
		// �����̉��[�̕ǁiy���W�����������j
		int index = Math::Rand(info.x + 2, info.x + info.w - 3);
		mMapData[info.y + info.h - 1][index].typeId = TileType::eEntrance;
	}
	else if (dir == Direction::eEast)
	{
		// �����̉E�[�̕�(x���W���傫����)
		int index = Math::Rand(info.y + 2, info.y + info.h - 3);
		mMapData[index][info.x].typeId = TileType::eEntrance;
	}
	else if (dir == Direction::eWest)
	{
		// �����̍��[�̕� (x���W����������)
		int index = Math::Rand(info.y + 2, info.y + info.h - 3);
		mMapData[index][info.x + info.w - 1].typeId = TileType::eEntrance;
	}
}


// �����̏���ݒ�
void CDungeonMap::SetRoomParameters(RoomInfo& info)
{
	// �����̕���ݒ�
	info.w = Math::Rand(MIN_ROOM_SIZE, MAX_ROOM_WIDTH);
	// �����A�����̏ꍇ�A�����I�Ɋ�ɂ���
	if (info.w % 2 == 0) info.w++;

	// �����̉��s��ݒ�;
	info.h = Math::Rand(MIN_ROOM_SIZE, MAX_ROOM_HEIGHT);
	// �����A�����̏ꍇ�A�����I�Ɋ�ɂ���
	if (info.h % 2 == 0) info.h++;

	// �z�u�ł���ő���W��ݒ�
	int maxX = ROOM_WIDTH - info.w - 1;
	int maxY = ROOM_HEIGHT - info.h - 1;

	// ������z�u����J�n���W (�����) �������_���Ɍ���
	info.x = Math::Rand(1, maxX);
	info.y = Math::Rand(1, maxY);

	// �z�u�J�n���W����ɋ�������
	if (info.x % 2 == 0) info.x++;
	if (info.y % 2 == 0) info.y++;

	// info.x/Y�𒲐��������ʁAmaxX/maxY���킸���ɒ����Ă��܂��\�������邽�߁A
	// �z�����ꍇ�́A2���炵��(���ۂ���)�͈͓��Ɏ��߂�
	if (info.x > maxX) info.x -= 2;
	if (info.y > maxY) info.y -= 2;
}

// ���̏�����
void CDungeonMap::InitializeSection()
{
	for (int y = 0; y < ROOM_HEIGHT; y++)
	{
		for (int x = 0; x < ROOM_WIDTH; x++)
		{
			mMapData[y][x].typeId = TileType::None;
		}
	}
}

// �����̏��̐ݒ�
void CDungeonMap::CreateRoomFloor(const RoomInfo& info)
{
	// ����ݒ�
	for (int y = info.y + 1; y < info.y + info.h - 1; y++)
	{
		for (int x = info.x + 1; x < info.x + info.w - 1; x++)
		{
			mMapData[y][x].typeId = TileType::eFloor;
		}
	}
}

// �����̕ǂ̐ݒ�
void CDungeonMap::CreateRoomWall(const RoomInfo& info)
{
	// �㉺�̕ǂ̐ݒ�
	for (int x = info.x + 1; x < info.x + info.w - 1; x++)
	{
		// ��̕�
		mMapData[info.y][x].typeId = TileType::eWall;
		mMapData[info.y][x].dir = Direction::eSouth;

		// ���̕�
		mMapData[info.y + info.h - 1][x].typeId = TileType::eWall;
		mMapData[info.y + info.h - 1][x].dir = Direction::eNorth;

		// ����x�̒l����Ȃ�
		if (x % 2 == 1)
		{
			// ��̕ǂ̍��W��ǉ�
			mEntranceCandidates.push_back({ x, info.y });

			// ���̕ǂ̍��W��ǉ�
			mEntranceCandidates.push_back({ x, info.y + info.h - 1 });
		}
	}
	// ���E�̕ǂ�ݒ�
	for (int y = info.y + 1; y < info.y + info.h - 1; y++)
	{
		// ���̕�
		mMapData[y][info.x].typeId = TileType::eWall;
		mMapData[y][info.x].dir = Direction::eEast;

		// �E�̕�
		mMapData[y][info.x + info.w - 1].typeId = TileType::eWall;
		mMapData[y][info.x + info.w - 1].dir = Direction::eWest;

		// ����y�̒l����Ȃ�
		if (y % 2 == 1)
		{
			// ���̕ǂ̍��W��ǉ�
			mEntranceCandidates.push_back({ info.x, y });

			// �E�̕ǂ̍��W��ǉ�
			mEntranceCandidates.push_back({ info.x + info.w - 1, y });
		}
	}
}

// �����̎l���̒���ݒ�
void CDungeonMap::CreateRoomPillar(const RoomInfo& info)
{
	mMapData[info.y][info.x].typeId = TileType::ePillar;	// ����
	mMapData[info.y][info.x].dir = Direction::eNorthWest;	// �k��
	mMapData[info.y][info.x + info.w - 1].typeId = TileType::ePillar;	// �E��
	mMapData[info.y][info.x + info.w - 1].dir = Direction::eNorthEast;	// �k��
	mMapData[info.y + info.h - 1][info.x].typeId = TileType::ePillar;	// ����
	mMapData[info.y + info.h - 1][info.x].dir = Direction::eSouthWest;	// �쐼
	mMapData[info.y + info.h - 1][info.x + info.w - 1].typeId = TileType::ePillar;	// �E��
	mMapData[info.y + info.h - 1][info.x + info.w - 1].dir = Direction::eSouthEast;	// �쓌
}


// �����̏o�����̐ݒ�
void CDungeonMap::CreateRoomEntrance(const RoomInfo& info)
{
	// �����̏o�����̐���ݒ�
	int numEntrances = Math::Rand(1, MAX_DOOR);

	// ��⃊�X�g�̃T�C�Y���擾
	int currentSize = static_cast<int>(mEntranceCandidates.size());
	// ���������������ďo�����̌�␔�����Ȃ��ꍇ�ɑΉ�
	numEntrances = std::min(numEntrances, currentSize);

	// ��������o�����������Ȃ�܂�
	while (numEntrances > 0)
	{
		// 0����ő�C���f�b�N�X�܂ł̗����𐶐�
		int randomIndex = Math::Rand(0, currentSize - 1);
		// �����_���ɑI�΂ꂽ���W���擾
		Point selectedDoorPos = mEntranceCandidates[randomIndex];
		// ���̈ʒu�̃^�C�����o�����ɏ㏑��
		mMapData[selectedDoorPos.y][selectedDoorPos.x].typeId = TileType::eEntrance;

		// �o�����ɕύX�����f�[�^�����X�g�ɒǉ�
		mEntrances.push_back(selectedDoorPos);

		// �ǂ���o�����ɏ㏑�������v�f�����X�g�̖����Ɠ���ւ���
		mEntranceCandidates[randomIndex] = mEntranceCandidates.back();
		// �����̗v�f���폜����
		mEntranceCandidates.pop_back();
		// ���X�g�̃T�C�Y���X�V
		currentSize--;
		// ��������o�����̃J�E���g�����炷
		numEntrances--;
	}
}

#if _DEBUG
// ���̃^�C���̃f�o�b�N�\��
void CDungeonMap::PrintSection()
{
	for (int y = 0; y < ROOM_HEIGHT; y++)
	{
		for (int x = 0; x < ROOM_WIDTH; x++)
		{
			if (mMapData[y][x].typeId == TileType::None) printf("0");
			else if (mMapData[y][x].typeId == TileType::eFloor) printf("1");
			else if (mMapData[y][x].typeId == TileType::eWall) printf("2");
			else if (mMapData[y][x].typeId == TileType::ePillar) printf("3");
			else if (mMapData[y][x].typeId == TileType::eEntrance) printf("4");
		}
		printf("\n");
	}
}
#endif