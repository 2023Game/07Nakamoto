#pragma once
#include "CTask.h"
#include "CRoom.h"

// �������Ǘ�����N���X
class CRoomManager : public CTask
{
public:
	// �C���X�^���X���擾
	static CRoomManager* Instance();

	// ���������X�g�ɒǉ�
	void Add(CRoom* room);
	// ���������X�g�����菜��
	void Remove(CRoom* room);

	// �w�肵���I�u�W�F�N�g�������Ă��镔����Ԃ�
	CRoom* GetCurrentRoom(CObjectBase* obj) const;

#if _DEBUG
	// ������\�����邩�ǂ���
	bool IsShowRoom() const;
	// �`��
	void Render() override;
#endif


private:
	// �R���X�g���N�^
	CRoomManager();
	// �f�X�g���N�^
	~CRoomManager();

	// CRoomManager�̃C���X�^���X
	static CRoomManager* spInstance;
	std::vector<CRoom*> mRooms;	// �����̃��X�g

#if _DEBUG
	bool mIsShowRoom;			// ������\�����邩�ǂ���
#endif
};