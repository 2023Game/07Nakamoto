#include "CDangeonMap.h"
#include "Maths.h"
#include "CFloor.h"
#include "CDoor.h"
#include "CWall.h"
#include "CPillar.h"
#include "CEntrance.h"

// �������镔���̕��ƍ����̍ŏ��l
#define MIN_ROOM_SIZE 3

// �������镔���̕��Ɖ��s�̍ő�l
// ���T�C�Y��-2���ė��[�̗���󂯂������̍ő�l��ݒ肷��
// ���T�C�Y�������̏ꍇ�͊�ɂ��邽�߂�-3�����ĕ����̍ő�l�Ƃ���
#define MAX_ROOM_WIDTH (ROOM_WIDTH % 2 == 0 ? ROOM_WIDTH - 3 : ROOM_WIDTH - 2)
#define MAX_ROOM_HEIGHT (ROOM_HEIGHT % 2 == 0 ? ROOM_HEIGHT - 3 : ROOM_HEIGHT -2)

// �����̔��̍ő吔
#define MAX_DOOR 3

#define TILE_SIZE 20.0f	// �^�C�����f���̑傫��

// �R���X�g���N�^
CDangeonMap::CDangeonMap()
{
	// ���ɕύX������i�[�p���X�g�̏�����
	mDoorCandidates.clear();

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
	// �C�����̔��̐ݒ�
	CreateRoomDoor(info);

#if _DEBUG
	// ���^�C���̃f�o�b�O�\��
	PrintSection();
#endif
}

// �f�X�g���N�^
CDangeonMap::~CDangeonMap()
{
}

// �����̏���ݒ�
void CDangeonMap::SetRoomParameters(RoomInfo& info)
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
void CDangeonMap::InitializeSection()
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
void CDangeonMap::CreateRoomFloor(const RoomInfo& info)
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
void CDangeonMap::CreateRoomWall(const RoomInfo& info)
{
	// �㉺�̕ǂ̐ݒ�
	for (int x = info.x + 1; x < info.x + info.w - 1; x++)
	{
		mMapData[info.y][x].typeId = TileType::eWall;
		mMapData[info.y + info.h - 1][x].typeId = TileType::eWall;
		
		// ����x�̒l����Ȃ�
		if (x % 2 == 1)
		{
			// ��̕ǂ̍��W��ǉ�
			mDoorCandidates.push_back({ x, info.y });

			// ���̕ǂ̍��W��ǉ�
			mDoorCandidates.push_back({ x, info.y + info.h - 1 });
		}

	}
	// ���E�̕ǂ�ݒ�
	for (int y = info.y + 1; y < info.y + info.h - 1; y++)
	{
		mMapData[y][info.x].typeId = TileType::eWall;
		mMapData[y][info.x + info.w - 1].typeId = TileType::eWall;

		// ����y�̒l����Ȃ�
		if (y % 2 == 1)
		{
			// ���̕ǂ̍��W��ǉ�
			mDoorCandidates.push_back({ info.x, y });

			// �E�̕ǂ̍��W��ǉ�
			mDoorCandidates.push_back({ info.x + info.w - 1, y });
		}
	}
}

// �����̎l���̒���ݒ�
void CDangeonMap::CreateRoomPillar(const RoomInfo& info)
{
	mMapData[info.y][info.x].typeId = TileType::ePillar;	// ����
	mMapData[info.y][info.x + info.w - 1].typeId = TileType::ePillar;  // �E��
	mMapData[info.y + info.h - 1][info.x].typeId = TileType::ePillar;  // ����
	mMapData[info.y + info.h - 1][info.x + info.w - 1].typeId = TileType::ePillar;	 // �E��
}

// �����̔��̐ݒ�
void CDangeonMap::CreateRoomDoor(const RoomInfo& info)
{
	// �����̔��̐���ݒ�
	int numDoors = Math::Rand(1, MAX_DOOR);

	// ��⃊�X�g�̃T�C�Y���擾
	int currentSize = static_cast<int>(mDoorCandidates.size());
	// ���������������Ĕ���␔�����Ȃ��ꍇ�ɑΉ�
	numDoors = std::min(numDoors, currentSize);

	// ����������������Ȃ�܂�
	while (numDoors > 0)
	{
		// ��⃊�X�g�̍ő�C���f�b�N�X���擾
		int maxIndex = currentSize - 1;

		// 0����ő�C���f�b�N�X�܂ł̗����𐶐�
		int randomIndex = Math::Rand(0, maxIndex);

		// �����_���ɑI�΂ꂽ���W���擾
		Point selectedDoorPos = mDoorCandidates[randomIndex];

		// ���̈ʒu�̃^�C������ɏ㏑��
		mMapData[selectedDoorPos.y][selectedDoorPos.x].typeId = TileType::eDoor;

		// �ǂ�����ɏ㏑�������v�f�����X�g�̖����Ɠ���ւ���
		mDoorCandidates[randomIndex] = mDoorCandidates.back();

		// �����̗v�f���폜����
		mDoorCandidates.pop_back();

		// ���X�g�̃T�C�Y���X�V
		currentSize--;

		// ����������̃J�E���g�����炷
		numDoors--;
	}
}

#if _DEBUG
// ���̃^�C���̃f�o�b�N�\��
void CDangeonMap::PrintSection()
{
	for (int y = 0; y < ROOM_HEIGHT; y++)
	{
		for (int x = 0; x < ROOM_WIDTH; x++)
		{
			if (mMapData[y][x].typeId == TileType::None) printf("0");
			else if (mMapData[y][x].typeId == TileType::eFloor) printf("1");
			else if (mMapData[y][x].typeId == TileType::eWall) printf("2");
			else if (mMapData[y][x].typeId == TileType::ePillar) printf("3");
			else if (mMapData[y][x].typeId == TileType::eDoor) printf("4");
		}
		printf("\n");
	}
}
#endif