#pragma once
#include "CDungeonMap.h"

#define DUNGEON_SECTION_X 4	// ���̋�搔
#define DUNGEON_SECTION_Y 4	// �c�̋�搔

class CDungeonManeger
{
public:
	// �R���X�g���N�^
	CDungeonManeger();
	// �f�X�g���N�^
	~CDungeonManeger();

	// �_���W��������
	void GenerateDungeon();
	// ���̎擾
	const CDungeonMap* GetSection(int x, int y) const;

private:
	// �����ƕ����̏o�������q����
	//void ConnectRooms();

	/// <summary>
	/// �ʘH�����q����
	/// </summary>
	/// <param name="aEntrance">�q����o����A</param>
	/// <param name="bEntrance">�q����o����B</param>
	/// <param name="dir">���p</param>
	//void ConnectBetweenRooms(CDungeonMap* entranceA, CDungeonMap* entranceB, CDungeonMap::Direction dir);

	/// <summary>
	/// �������m�̊Ԃ̒ʘH���쐬
	/// </summary>
	/// <param name="aPos"></param>
	/// <param name="bPos"></param>
	/// <param name="entranceA">�q����o����A</param>
	/// <param name="entranceB">�q����o����B</param>
	/// <param name="dir">���p</param>
	//void CreateCorridorBetween(
	//	const CDungeonMap::Point& aPos, const CDungeonMap::Point& bPos,
	//	CDungeonMap* entranceA, CDungeonMap* entranceB,
	//	CDungeonMap::Direction dir);

	// �S�̂̋��̔z��
	CDungeonMap* mpSections[DUNGEON_SECTION_Y][DUNGEON_SECTION_X];
};