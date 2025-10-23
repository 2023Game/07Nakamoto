#pragma once

class CBpsMap
{
public:
	// �R���X�g���N�^
	CBpsMap(int x,int y);
	// �f�X�g���N�^
	~CBpsMap();

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

	// �����̏��
	struct Room
	{
		int x, y;			// �����̍���̍��W
		int width, height;	// �����̃T�C�Y

		TileType type;		// �^�C���̎��
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

private:
	// �m�[�h�̍폜
	void DeleteNode(SectionNode* node);

	// ������
	void Initialize(int width, int height);

	// ��敪��
	void Split(SectionNode* node);
	// ��悲�Ƃɕ����̐ݒ�
	void PlaceRoom(SectionNode* node, std::vector<std::vector<Room>>& map);

#if _DEBUG
	// 2�����z��̃f�o�b�O�\��
	void PrintSection();
#endif // _DEFU


	// ���[�g�m�[�h(�ŏ��̋��)
	SectionNode* mpRoot;
	// �Q�����z��(�ϒ��z��)�̃}�b�v�f�[�^
	std::vector<std::vector<Room>> mMapData;
};