#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H

#include "CTaskManager.h"

class CCollisionManager :public CTaskManager
{
public:
	//�C���X�^���X�̎擾
	static CCollisionManager* GetInstance();
	//�Փˏ���
	void Collison();
private:
	//�}�l�[�W���̃C���X�^���X
	static CCollisionManager* mpInstance;
};

#endif // !CCOLLISIONMANAGER_H

