#pragma once

#define ROOM_WIDTH 15	// �P���̕�
#define ROOM_HEIGHT 15	// �P���̉��s

#define TILE_SIZE 20.0f	// �^�C�����f���̑傫��

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
	// �����̏o�����̃��X�g���擾
	std::vector<Point> GetEntrances() const;

	// �^�C���^�C�v�̐ݒ�
	void SetTileType(int x, int y, TileType type);

private:
	// �����̏���ݒ�
	void SetRoomParameters(RoomInfo& info);

	// ���̃f�[�^�z��
	Tile mMapData[ROOM_HEIGHT][ROOM_WIDTH];

	// �o�����ɕύX������i�[�p���X�g
	std::vector<Point> mEntranceCandidates;
	// �����̏o������ۑ����邽�߂̃��X�g
	std::vector<Point> mEntrances;

	// ���̏�����
	void InitializeSection();

	// �����̏��̐ݒ�
	void CreateRoomFloor(const RoomInfo& info);
	// �����̕ǂ̐ݒ�
	void CreateRoomWall(const RoomInfo& info);
	// �����̎l���̒���ݒ�
	void CreateRoomPillar(const RoomInfo& info);
	// �����̏o�����̐ݒ�
	void CreateRoomEntrance(const RoomInfo& info);

#if _DEBUG
	// ���̃^�C���̃f�o�b�N�\��
	void PrintSection();
#endif
};