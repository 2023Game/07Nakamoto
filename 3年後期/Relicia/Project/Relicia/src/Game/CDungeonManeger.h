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
	// �o�������m���q����ʘH�𐶐�
	void ConnectRooms();

	CDungeonMap* mpSections[DUNGEON_SECTION_Y][DUNGEON_SECTION_X];
};