#pragma once

#define DUNGEON_SECTION_X 4	// ���̋�搔
#define DUNGEON_SECTION_Y 4	// �c�̋�搔

class CDungeonMap;

class CDungeonManeger
{
public:
	// �R���X�g���N�^
	CDungeonManeger();
	// �f�X�g���N�^
	~CDungeonManeger();

	// �S�̋��̏�����
	void Initialize();

	// ���̎擾
	const CDungeonMap* GetSection(int x, int y) const;

private:
	// �S�̂̋��̉ϒ��z��
	std::vector<std::vector<CDungeonMap*>>mpSections;

#if _DEBUG
	// �S�̂̋��̃^�C���̃f�o�b�N�\��
	void PrintSection();
#endif

};