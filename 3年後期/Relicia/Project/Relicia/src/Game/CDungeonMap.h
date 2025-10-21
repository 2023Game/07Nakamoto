#pragma once
#include "CDungeonManeger.h"

#define TILE_SIZE 20.0f	// �^�C�����f���̑傫��

#define ROOM_WIDTH 15	// �P���̕�
#define ROOM_HEIGHT 15	// �P���̉��s

class CDungeonMap
{
public:
	// �R���X�g���N�^
	CDungeonMap();
	// �f�X�g���N�^
	~CDungeonMap();

	// ������\���f�[�^
	enum class Direction
	{
		eNorth,	// �k
		eSouth,	// ��
		eEast,	// ��
		eWest,	// ��

		eNorthEast,	// �k��
		eSouthEast,	// �쓌
		eSouthWest,	// �쐼
		eNorthWest,	// �k��
	};

	// �^�C���̎��
	enum class TileType
	{
		None,		// ����
		eFloor,		// ��
		eWall,		// ��
		eEntrance,	// �o����
		ePillar,	// ��
		ePassage,	// �ʘH
		eDoor,		// ��
	};

	// �^�C���̏��
	struct Tile
	{
		TileType typeId = TileType::None;	// �^�C���̎��
		Direction dir = Direction::eNorth;	// ����(�ǂ���̉�]�p)
	};

	// ���̃f�[�^
	struct RoomInfo
	{
		// �}�b�v�̃C���f�b�N�X(������̍��W)
		int x;
		int y;

		// �����̃T�C�Y(���Ɖ��s)
		int w;
		int h;
	};

	// �ǂ̔z��̃f�[�^
	struct Point
	{
		int x, y;
	};

	// �^�C���̏����擾
	const Tile& GetTile(int x,int y) const;

	// �����̕������X�g���擾
	std::vector<RoomInfo> GetRooms() const;
	// �����̏o�����̃��X�g���擾
	std::vector<Point> GetEntrances() const;

	// �^�C���^�C�v�̐ݒ�
	void SetTileType(int x, int y, TileType type);
	// �����̏o�����̐ݒ�(�o�����������p���w��)
	void CreateRoomEntrance(Direction dir);

	std::vector<Point> GetmEntrances();

private:
	// �����̕������X�g
	std::vector<RoomInfo> mRooms;
	// ���̃f�[�^�z��
	std::vector<std::vector<Tile>> mMapData;

	// �o�����ɕύX������i�[�p���X�g
	std::vector<Point> mEntranceCandidates;
	// �����̏o������ۑ����邽�߂̃��X�g
	std::vector<Point> mEntrances;

	// ��惊�X�g�̏�����
	void Initialize(int width, int height);

	// �����̏���ݒ�
	void SetRoomParameters(RoomInfo& info);
	// �����̏��̐ݒ�
	void CreateRoomFloor(const RoomInfo& info);
	// �����̕ǂ̐ݒ�
	void CreateRoomWall(const RoomInfo& info);
	// �����̎l���̒���ݒ�
	void CreateRoomPillar(const RoomInfo& info);

	// �o�������m���q����
	void ConnectPassage(CDungeonMap* a, CDungeonMap* b);

	// �����̏o�����̐ݒ�
	void CreateRoomEntrance(const RoomInfo& info);

#if _DEBUG
	// 1���̃^�C���̃f�o�b�N�\��
	void PrintSection();
#endif
};