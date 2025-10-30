#pragma once

class CBspMap
{
public:
	// �R���X�g���N�^
	CBspMap(int x,int y);
	// �f�X�g���N�^
	~CBspMap();

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

		eBoundary   // ���̋��E��
	};

	// ������\���f�[�^
	enum class Direction
	{
		None,

		eNorth,	// �k(y-1�̕���)
		eSouth,	// ��(y+1�̕���)
		eEast,	// ��(x+1�̕���)
		eWest,	// ��(x-1�̕���)

		eNorthEast,	// �k��
		eSouthEast,	// �쓌
		eSouthWest,	// �쐼
		eNorthWest,	// �k��
	};

	// �^�C���̏��
	struct Tile
	{
		TileType type;	// �^�C���̎��
		Direction dir;	// ����
		bool passage;	// �ʘH���ǂ���
		bool passageWall;	// �ʘH�̕ǂ𐶐�������
	};

	// �����̏��
	struct Room
	{
		int x, y;			// �����̍���̒��̍��W
		int width, height;	// �����̃T�C�Y

		// �����̎��
		enum class RoomType
		{
			eNormal,	// �ʏ핔��
			eBoss,		// �{�X����
			eEntrance,	// �J�n����
			eExit,		// �E�o����
		};

		RoomType type;

		bool connected = false;	// �q�����Ă��邩
	};

	// ���m�[�h
	struct SectionNode
	{
		int x, y;			// ���̍�����W
		int width, height;	// ���T�C�Y

		SectionNode* left;	// ���i�܂��͏�j�q�m�[�h
		SectionNode* right;	// �E�i�܂��͉��j�q�m�[�h

		Room room; // �����f�[�^
	};

	// BSP�}�b�v�f�[�^�̎擾
	const std::vector<std::vector<Tile>>& GetTileData()const;
	// �ʘH�̕ǂ𐶐��t���O��true�ɂ���
	void SetPassageWall(int x,int y);

	// �����̓����ŕǂɋ߂���
	bool IsNearRoomWall(const CVector2& pos);

private:
	// �m�[�h�̍폜
	void DeleteNode(SectionNode* node);

	// ������
	void Initialize(int width, int height);

	// ��敪��
	void Split(SectionNode* node);

	// ��悲�Ƃɕ����̐ݒ�
	void PlaceRoom(SectionNode* node, std::vector<std::vector<Tile>>& map);

	// �����̏��f�[�^�̐ݒ�
	void CreateRoomFloor(SectionNode* node, std::vector<std::vector<Tile>>& map);
	// �����̕ǃf�[�^�̐ݒ�
	void CreateRoomWall(const Room& room, std::vector<std::vector<Tile>>& map);
	// �����̎l���f�[�^�̒���ݒ�
	void CreateRoomPillar(const Room& room, std::vector<std::vector<Tile>>& map);

	// �����K�w�̕������m��ʘH�Ōq����
	void ConnectRooms(SectionNode* node, std::vector<std::vector<Tile>>& map);

	// �����̃����_���ȍ��W���擾
	CVector2 GetRoomRandomPos(SectionNode* node);

	// �������m�̒ʘH�f�[�^�̐ݒ�
	void CreatePassage(std::vector<std::vector<Tile>>& map, CVector2 a, CVector2 b);

	// ���p�̐����΂�Ԃ�
	Direction InverseDirection(Direction dir) const;

#if _DEBUG
	// ���̋��E���ݒ�
	void DrawBoundary(SectionNode* node, std::vector<std::vector<Tile>>& map);
	// 2�����z��̃f�o�b�O�\��
	void PrintSection();
#endif // _DEFU

	// ���[�g�m�[�h(�ŏ��̋��)
	SectionNode* mpRoot;
	// �Q�����z��(�ϒ��z��)�̃}�b�v�f�[�^
	std::vector<std::vector<Tile>> mMapData;
};