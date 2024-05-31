#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H

#include "CTaskManager.h"
#include "CCollider.h"

//�Փˏ����͈͂�藣��Ă���R���C�_�͏Փˏ��������Ȃ�
#define COLLISIONRANGE 100

class CCollisionManager :public CTaskManager
{
public:
	//�C���X�^���X�̎擾
	static CCollisionManager* GetInstance();
	//�Փˏ���
	void Collison();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="collider">�R���C�_</param>
	/// <param name="range">�͈�</param>
	void Collision(CCollider* c, int range);
private:
	//�}�l�[�W���̃C���X�^���X
	static CCollisionManager* mpInstance;
};

#endif // !CCOLLISIONMANAGER_H

