#pragma once

#define ROOM_WIDTH 15	// ���̕�
#define ROOM_HEIGHT 15	// ���̉��s

#define TILE_SIZE 20.0f	// �^�C�����f���̑傫��

class CDangeonMap
{
public:
	// �R���X�g���N�^
	CDangeonMap();
	// �f�X�g���N�^
	~CDangeonMap();

	// ������\���f�[�^
	enum class Direction
	{
		eNorth,	// �k
		eSouth,	// ��
		eEast,	// ��
		eWest,	// ��

	};

	// �^�C���̎��
	enum class TileType
	{
		None,		// ����
		eFloor,		// ��
		eWall,		// ��
		eEntrance,	// �o����
		eDoor,		// ��
		ePillar,	// ��
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
private:
	// �����̏���ݒ�
	void SetRoomParameters(RoomInfo& info);

	// ���̃f�[�^�z��
	Tile mMapData[ROOM_HEIGHT][ROOM_WIDTH];
	// ���ɕύX������i�[�p���X�g
	std::vector<Point> mDoorCandidates;

	// ���̏�����
	void InitializeSection();

	// �����̏��̐ݒ�
	void CreateRoomFloor(const RoomInfo& info);
	// �����̕ǂ̐ݒ�
	void CreateRoomWall(const RoomInfo& info);
	// �����̎l���̒���ݒ�
	void CreateRoomPillar(const RoomInfo& info);
	// �����̔��̐ݒ�
	void CreateRoomDoor(const RoomInfo& info);

#if _DEBUG
	// ���̃^�C���̃f�o�b�N�\��
	void PrintSection();
#endif
};